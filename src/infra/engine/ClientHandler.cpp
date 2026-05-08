/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 20:01:26 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/07 21:34:16 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra/engine/ClientHandler.hpp"
#include "infra/engine/ServerEngine.hpp"
#include <vector>

ClientHandler::ClientHandler(const int fd,
							 const char* ip,
							 uint16_t port,
							 ServerEngine *serverEngine) {
	_fd = fd;
	_ip = std::string(ip);
	_port = port;
	_serverEngine = serverEngine;
	_httpProcessor = _serverEngine->getHttpProcFactory()->createProcessor(_ip, _serverEngine->getConfig()->getPort());
	_writeBuffer = "";
	_writeOffset = 0;
	_closeAfterWrite = false;
	_lastActivity = time(NULL);
}

ClientHandler::~ClientHandler(void) {
}

void ClientHandler::event(epoll_event &ev) {
	if (ev.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
		closeConnection();
		return ;
	}

	if (ev.events & EPOLLIN)
		onReading();

	if (ev.events & EPOLLOUT)
		onWriting();
}

void ClientHandler::onReading(void) {
	const size_t bufSize = STREAMING_BUFFER_SIZE * 1024 + 1;
	char buffer[bufSize];
	ssize_t rbytes = recv(_fd, buffer, bufSize, 0);

	_lastActivity = time(NULL);
	if (rbytes == 0) {
		std::cerr << "client desconected: " << _ip << ":" << _serverEngine->getConfig()->getPort() << std::endl;
		closeConnection();
		return ;
	} else if (rbytes < 0) {
		std::cerr << "error server" << std::endl;
		closeConnection();
		return ;
	}

	_httpProcessor->feedChunk(buffer, static_cast<size_t>(rbytes));
	if (_httpProcessor->isReady() || _httpProcessor->hasError())
		prepareResponse();
}

void ClientHandler::onWriting(void) {
	const char *data = NULL;
	size_t remaning = 0;
	ssize_t nsent = 0;

	_lastActivity = time(NULL);
	if (_writeBuffer.empty() || _writeOffset >= _writeBuffer.size()) {
		if (_closeAfterWrite) {
			closeConnection();
			return ;
		}
		_serverEngine->changeHandlerState(_fd, EPOLLIN);
		return ;
	}

	data = _writeBuffer.data() + _writeOffset;
	remaning = _writeBuffer.size() - _writeOffset;
	nsent = send(_fd, data, remaning, MSG_NOSIGNAL);

	if (nsent < 0) {
		closeConnection();
		return ;
	}

	_writeOffset += static_cast<size_t>(nsent);
	if (_writeOffset >= _writeBuffer.size()) {
		if (_closeAfterWrite) {
			closeConnection();
			return ;
		}

		_writeBuffer.clear();
		_writeOffset = 0;
		_httpProcessor->reset();
		_serverEngine->changeHandlerState(_fd, EPOLLIN);
	}
}

void ClientHandler::prepareResponse(void) {
	const std::vector<char>& response = _httpProcessor->buildResponse();

	_writeBuffer.erase(0, _writeOffset);
	_writeOffset = 0;
	_writeBuffer.assign(response.begin(), response.end());

	_closeAfterWrite = _httpProcessor->shouldCloseConnection() || _httpProcessor->hasError();
	_serverEngine->changeHandlerState(_fd, EPOLLOUT);
}

void ClientHandler::closeConnection(void) {
	_serverEngine->removeHandler(_fd);
}

bool ClientHandler::isTimeout(time_t now) const {
	return (now - _lastActivity) > TIMEOUT;
}

void ClientHandler::onTimeout(void) {
	// TODO: sendTimeoutResponse (IHttpProcessor error pages/responses)
}
