/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AcceptHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 13:54:52 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/08 14:51:34 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra/engine/AcceptHandler.hpp"
#include "infra/engine/ServerEngine.hpp"
#include "infra/engine/ClientHandler.hpp"

AcceptHandler::AcceptHandler(ServerEngine* serverEngine) {
	_serverEngine = serverEngine;
	_fd = serverEngine->getSocketFd();
	_stage = ACCEPTING;
}

AcceptHandler::~AcceptHandler(void) {
}

void AcceptHandler::event(epoll_event&) {
	struct sockaddr_in	addr;
	socklen_t			addrLen = sizeof(addr);
	IEventHandler*		client = NULL;
	int					fd = 0, flags = 0;
	uint16_t			port = 0;
	char				ipBuffer[INET_ADDRSTRLEN] = {0};

	if ((fd = accept(_serverEngine->getSocketFd(), reinterpret_cast<sockaddr*>(&addr), &addrLen)) < 0) {
		std::cerr << "accept" << std::endl; // TODO: insert in the log
		return ;
	}

	port = ntohs(addr.sin_port);
	if (inet_ntop(AF_INET, &addr.sin_addr, ipBuffer, sizeof(ipBuffer)) == NULL) {
		std::cerr << "inet_ntop" << std::endl;
		close(fd);
		return ;
	}
	client = new ClientHandler(fd, ipBuffer, port, _serverEngine);
	std::cout << "client connected: " << ipBuffer << ":" << port << std::endl;

	flags = fcntl(fd, F_GETFL, 0);
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
		std::cerr << "fcntl" << std::endl; // TODO: create exceptio
		delete client;
		close(fd);
		return ;
	}

	_serverEngine->addHandler(fd, EPOLLIN, client);
}

void AcceptHandler::closeConnection(void) {
	_stage = CLOSED;
}

bool AcceptHandler::isTimeout(time_t) const {
	return false;
}

void AcceptHandler::onTimeout(void) {
}

AcceptHandler::Stage AcceptHandler::stage(void) const {
	return _stage;
}
