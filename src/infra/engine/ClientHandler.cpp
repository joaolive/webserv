/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 20:01:26 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/11 22:03:46 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra/engine/ClientHandler.hpp"

#include "infra/engine/ServerEngine.hpp"

ClientHandler::ClientHandler(const int fd,
							 const std::string ip,
							 uint16_t port,
							 ServerEngine *serverEngine) 
	:	_fd(fd), 
		_ip(ip), 
		_port(port), 
		_serverEngine(serverEngine), 
		_writeBuffer(""), 
		_writeOffset(0), 
		_closeAfterWrite(false), 
		_lastActivity(0), 
		_stage(READING), 
		_httpProcessor(_serverEngine->getHttpProcFactory()
									->createProcessor(_ip, _port)) {
}

ClientHandler::~ClientHandler(void) {
	delete _httpProcessor;
}

void ClientHandler::event(epoll_event &ev) {
	if (ev.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
		return closeConnection(ERROR);

	if (ev.events & EPOLLIN)
		onReading();
	else if (ev.events & EPOLLOUT)
		onWriting();
}

void ClientHandler::onReading(void) {
	const size_t	bufSize = STREAMING_BUFFER_SIZE * 1024 + 1;
	char			buffer[bufSize] = {0};
	ssize_t			rbytes = 0;

	if (_stage != READING)
		return ;

	_lastActivity = time(NULL);
	rbytes = recv(_fd, buffer, bufSize, 0);
	if (rbytes == 0) {
		return closeConnection(CLOSED);
	} else if (rbytes == -1) {
		if (!isTimeout(_serverEngine->getRegisterTime()))
			return ;

		closeConnection(ERROR);
		throw std::runtime_error("server error read request");
	}

	_httpProcessor->feedChunk(buffer, static_cast<size_t>(rbytes));
	if (_httpProcessor->isReady() || _httpProcessor->hasError())
		prepareResponse();
}

void ClientHandler::onWriting(void) {
	if (_stage != WRITING)
		return ;

	_lastActivity = time(NULL);

	if (_writeBuffer.empty() || _writeOffset >= _writeBuffer.size())
		return finishWriting();

	const char* data = _writeBuffer.data() + _writeOffset;
	size_t remaning = _writeBuffer.size() - _writeOffset;
	ssize_t nSent = send(_fd, data, remaning, MSG_NOSIGNAL);

	if (nSent < 0) {
		if (!isTimeout(_serverEngine->getRegisterTime()))
			return ;

		closeConnection(ERROR);
		throw std::runtime_error("server erro send response");
	}

	_writeOffset += static_cast<size_t>(nSent);
	if (_writeOffset >= _writeBuffer.size())
		finishWriting();
}

void ClientHandler::prepareResponse(void) {
	const std::vector<char>& response = _httpProcessor->buildResponse();

	_writeBuffer.assign(response.begin(), response.end());
	_writeOffset = 0;
	_stage = WRITING;

	_closeAfterWrite = _httpProcessor->shouldCloseConnection() || 
					   _httpProcessor->hasError();
	_serverEngine->changeHandlerState(_fd, EPOLLOUT);
}

void ClientHandler::finishWriting(void) {
	if (_closeAfterWrite)
		return closeConnection(CLOSED);

	_writeBuffer.clear();
	_writeOffset = 0;
	_httpProcessor->reset();
	_stage = READING;
	_serverEngine->changeHandlerState(_fd, EPOLLIN);
}

void ClientHandler::closeConnection(const Stage& stage) {
	if (_stage == stage)
		return ;

	if (stage == ERROR)
		std::cout << "client error ";
	else if (stage == CLOSED)
		std::cout << "client disconected ";

	std::cout << _ip << ":" << _port << std::endl;
	_stage = stage;
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
