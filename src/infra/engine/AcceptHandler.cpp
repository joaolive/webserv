/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AcceptHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 13:54:52 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/08 15:25:48 by mhidani          ###   ########.fr       */
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

ClientHandler* AcceptHandler::prepareClient(const int& socketFd) {
	struct sockaddr_in	addr;
	socklen_t			addrLen = sizeof(addr);
	int					clientFd = -1, flags = 0;
	uint16_t			port = 0;
	char				ip[INET_ADDRSTRLEN] = {0};
	ClientHandler*		client = NULL;
	
	clientFd = accept(socketFd, reinterpret_cast<sockaddr*>(&addr), &addrLen);
	if (clientFd < 0)
		return NULL;

	port = ntohs(addr.sin_port);
	if (inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip)) == NULL) {
		std::cerr << "inet_ntop" << std::endl;
		close(clientFd);
		return NULL;
	}

	client = new ClientHandler(clientFd, ip, port, _serverEngine);
	flags = fcntl(clientFd, F_GETFL, 0);
	if (fcntl(clientFd, F_SETFL, flags | O_NONBLOCK) < 0) {
		std::cerr << "fcntl" << std::endl; // TODO: create exceptio
		delete client;
		close(clientFd);
		return NULL;
	}
	return client;
}

void AcceptHandler::event(epoll_event&) {
	const int socketFd = _serverEngine->getSocketFd();
	
	while (true) {
		ClientHandler* client = prepareClient(socketFd);
		if (client == NULL)
			break;
		_serverEngine->addHandler(client->getFd(), EPOLLIN, client);
		std::cout << "client connected " << client->getIp() << ":" << client->getPort() << std::endl;
	}
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
