/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 15:32:18 by mhidani           #+#    #+#             */
/*   Updated: 2026/04/27 18:20:58 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientHandler.hpp"

ClientHandler::ClientHandler(
		const int& fd, 
		Reactor* eventLoop, 
		IHttpServer* http) {
	_fd = fd;
	_eventLoop = eventLoop;
	_http = http;
	_state = READING;
}

ClientHandler::~ClientHandler(void) {
}

void ClientHandler::handleEvent(epoll_event& event) {
	if (event.events & EPOLLIN) read();
	else if (event.events & EPOLLOUT) write();
}

int ClientHandler::read(void) {
	char	buffer[READ_BUFFER_SIZE];
	ssize_t	rbytes = 0;

	while ((rbytes = recv(_fd, buffer, READ_BUFFER_SIZE, 0)) > 0)
		_readBuffer.append(buffer, rbytes);
	if (rbytes < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
		closeConnection();
		return -1; // TODO: check to return message and code
	}

	if (_readBuffer.find("\r\n\r\n") != std::string::npos) {
		_eventLoop->changeHandlerToWrite(_fd);
		_state = WRITING;
		_writeBuffer = _http->process(_readBuffer);
	}
	return 0;
}

int ClientHandler::write(void) {
	ssize_t	wbytes = 1;
	int		statusCode = 0;

	while (!_writeBuffer.empty() && wbytes > 0) {
		wbytes = send(_fd, _writeBuffer.c_str(), _writeBuffer.size(), 0);
		_writeBuffer.erase(0, wbytes);
	}
	if (wbytes < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
		closeConnection();
		return -1; // TODO: check to return message and code
	}
	closeConnection();
	return 0;
}

void ClientHandler::closeConnection(void) {
	_eventLoop->removeHandler(_fd);
	_state = CLOSED;
}
