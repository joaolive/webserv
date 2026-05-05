/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AcceptHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 13:54:52 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/05 11:27:49 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra/engine/AcceptHandler.hpp"

AcceptHandler::AcceptHandler(const int& fd, 
							 ServerEngine* sv, 
							 IHttpProcessorFactory& fc)
	:	_fd(fd), 
		_server(sv), 
		_factory(fc) {
}

AcceptHandler::~AcceptHandler(void) {
}

void AcceptHandler::event(epoll_event& event) {
	struct sockaddr_in	addr;
	socklen_t			addrLen = sizeof(addr);
	IEventHandler*		client = NULL;
	int					fd = 0, flags = 0;
	uint16_t			port = 0;
	char			ipBuffer[INET_ADDRSTRLEN] = {0};

	if ((fd = accept(_server->getSocketFd(), reinterpret_cast<sockaddr*>(&addr), &addrLen)) < 0) {
		std::cerr << "accept" << std::endl; // TODO: insert in the log
		return ;
	}

	port = ntohs(addr.sin_port);
	if (inet_ntop(AF_INET, &addr.sin_addr, ipBuffer, sizeof(ipBuffer)) == NULL) {
		std::cerr << "inet_ntop" << std::endl;
		close(fd);
		return ;
	}
	client = new ClientHandler(fd, port, std::string(ipBuffer), _server, _factory);
	std::cout << "client connected: " << ipBuffer << ":" << port << std::endl;

	flags = fcntl(fd, F_GETFL, 0);
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
		std::cerr << "fcntl" << std::endl; // TODO: create exceptio
		return ;
	}

	_server->addHandler(fd, EPOLLIN, client);
}

void AcceptHandler::closeConnection(void) {
	_server->removeHandler(_fd);
}

bool AcceptHandler::isTimeout(time_t now) const {
	return false;
}

void AcceptHandler::onTimeout(void) {
}
