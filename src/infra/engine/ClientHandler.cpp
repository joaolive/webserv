/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 20:01:26 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/08 16:38:40 by mhidani          ###   ########.fr       */
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
	_ip = std::string(ip); // TODO: check
	_port = port; // TODO: check
	_serverEngine = serverEngine;
	_httpProcessor = _serverEngine->getHttpProcFactory()->createProcessor(_ip, _serverEngine->getConfig()->getPort()); // TODO: check
	_writeBuffer = "";
	_writeOffset = 0;
	_closeAfterWrite = false;
	_lastActivity = time(NULL);
	_stage = READING;
}

ClientHandler::~ClientHandler(void) {
	delete _httpProcessor;
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
	ssize_t rbytes = 0;

	if (_stage != READING)
		return ;

	rbytes = recv(_fd, buffer, bufSize, 0);
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
	if (_stage != WRITING)
		return ;

	_lastActivity = time(NULL);

	if (_writeBuffer.empty() || _writeOffset >= _writeBuffer.size()) {
		if (_closeAfterWrite) {
			closeConnection();
			return ;
		}
		_writeBuffer.clear();
		_writeOffset = 0;
		_httpProcessor->reset();
		_stage = READING;
		_serverEngine->changeHandlerState(_fd, EPOLLIN);
		return ;
	}

	const char* data = _writeBuffer.data() + _writeOffset;
	size_t remaning = _writeBuffer.size() - _writeOffset;
	ssize_t nsent = send(_fd, data, remaning, MSG_NOSIGNAL);

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
		_stage = READING;
		_serverEngine->changeHandlerState(_fd, EPOLLIN);
	}
}

void ClientHandler::prepareResponse(void) {
	const std::vector<char>& response = _httpProcessor->buildResponse();

	_writeBuffer.assign(response.begin(), response.end());
	_writeOffset = 0;
	_stage = WRITING;

	_closeAfterWrite = _httpProcessor->shouldCloseConnection() || _httpProcessor->hasError();
	_serverEngine->changeHandlerState(_fd, EPOLLOUT);
}

void ClientHandler::closeConnection(void) {
	if (_stage == CLOSED)
		return ;

	_stage = CLOSED;
	std::cout << "client disconected " << _ip << ":" << _port << std::endl;
}

bool ClientHandler::isTimeout(time_t now) const {
	return (now - _lastActivity) > TIMEOUT;
}

void ClientHandler::onTimeout(void) {
	// TODO: sendTimeoutResponse (IHttpProcessor error pages/responses)
}

ClientHandler::Stage ClientHandler::stage(void) const {
	return _stage;
}

int ClientHandler::getFd(void) const {
	return _fd;
}

std::string ClientHandler::getIp(void) const {
	return _ip;
}

uint32_t ClientHandler::getPort(void) const {
	return _port;
}
