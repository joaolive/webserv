/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 20:01:26 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/03 09:19:18 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientHandler.hpp"

#include "infra/ServerEngine.hpp"

ClientHandler::ClientHandler(int fd, 
							 uint32_t port, 
							 std::string ip, 
							 ServerEngine* sv, 
							 IHttpProcessorFactory& fc)
	:	_fd(fd), 
		_port(port), 
		_ip(ip), 
		_server(sv), 
		_processor(fc.createProcessor(ip, port)), 
		_writeBuffer(), 
		_writeOffset(0), 
		_closeAfterWrite(false) {
}

ClientHandler::~ClientHandler(void) {
	delete _processor;
}

void ClientHandler::event(epoll_event& ev) {
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
	const size_t	bufSize = STREAMING_BUFFER_SIZE * 1024 + 1;
	char			buffer[bufSize];
	ssize_t 		rbytes = recv(_fd, buffer, bufSize, 0);

	if (rbytes <= 0) {
		closeConnection();
		return ;
	}

	_processor->feedChunk(buffer, static_cast<size_t>(rbytes));
	if (_processor->isReady() || _processor->hasError())
		prepareResponse();
}

void ClientHandler::onWriting(void) {
	const char*	data = NULL;
	size_t		remaning = 0;
	ssize_t		nsent = 0;

	if (_writeBuffer.empty() || _writeOffset >= _writeBuffer.size()) {
		if (_closeAfterWrite) {
			closeConnection();
			return ;
		}
		_server->changeHandlerState(_fd, EPOLLIN);
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
			return;
		}

		_writeBuffer.clear();
		_writeOffset = 0;
		_processor->reset();
		_server->changeHandlerState(_fd, EPOLLIN);
	}
}

void ClientHandler::prepareResponse(void) {
	std::string	response = _processor->buildResponse();

	_writeBuffer.erase(0, _writeOffset);
	_writeOffset = 0;
	_writeBuffer += response;

	_closeAfterWrite = _processor->shouldCloseConnection() 
					|| _processor->hasError();
	_server->changeHandlerState(_fd, EPOLLOUT);
}

void ClientHandler::closeConnection(void) {
	_server->removeHandler(_fd);
}
