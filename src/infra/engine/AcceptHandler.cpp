/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AcceptHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 13:54:52 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/12 19:19:39 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra/engine/AcceptHandler.hpp"

#include "infra/engine/ServerEngine.hpp"
#include "infra/engine/ClientHandler.hpp"

AcceptHandler::AcceptHandler(ServerEngine* serverEngine) 
	:	_serverEngine(serverEngine), 
		_fd(serverEngine->getSocketFd()), 
		_stage(WAIT) {
}

AcceptHandler::~AcceptHandler(void) {
}

int AcceptHandler::acceptConnection(const int& fd, sockaddr_in& addr) {
	socklen_t	len = sizeof(addr);
	int			clientFd = accept(fd, reinterpret_cast<sockaddr*>(&addr), &len);

	return clientFd;
}

std::string AcceptHandler::networkToPresentation(const int& fd, sockaddr_in& addr) {
	char	ip[INET_ADDRSTRLEN] = {0};

	if (inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip)) == NULL) {
		close(fd);
		throw std::runtime_error("AcceptHandler::networkToPresentation");
	}
	return std::string(ip);
}

void AcceptHandler::changeToNoBlock(const int& fd) {
	int	flags = fcntl(fd, F_GETFL, 0);

	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
		close(fd);
		throw std::runtime_error("AcceptHandler::changeToNoBlock");
	}
}

bool AcceptHandler::acceptHandler(const int& socketFd) {
	struct sockaddr_in	addr;
	int					fd = 0;
	uint16_t			port = 0;
	std::string			ip;
	IEventHandler*		event = NULL;

	try {
		if ((fd = acceptConnection(socketFd, addr)) < 0) return false;
		port = ntohs(addr.sin_port);
		ip = networkToPresentation(fd, addr);
		changeToNoBlock(fd);
		event = new ClientHandler(fd, ip, port, _serverEngine);

		_serverEngine->addHandler(fd, EPOLLIN, event);

	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return false;
	}

	std::cout << "client connected " << ip << ":" << port << std::endl;
	return true;
}

void AcceptHandler::event(epoll_event& event) {
	if (event.events & (EPOLLERR | EPOLLHUP)) {
		std::cerr << "error on server socket[" << _fd << "]" << std::endl;
		return ;
	}

	while (acceptHandler(_fd)) ;
}

void AcceptHandler::closeConnection(const Stage& stage) {
	_stage = stage;
}

bool AcceptHandler::isTimeout(time_t) const {
	return false;
}

void AcceptHandler::onTimeout(void) {
}

AcceptHandler::Stage AcceptHandler::stage(void) const {
	return _stage;
}
