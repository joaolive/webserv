/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AcceptHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 13:54:52 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/03 09:17:43 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AcceptHandler.hpp"

#include "infra/ServerEngine.hpp"

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
	std::string			ip = NULL;

	if ((fd = accept(_server->getSocketFd(), reinterpret_cast<sockaddr*>(&addr), &addrLen)) < 0) {
		std::cerr << "accept" << std::endl; // TODO: insert in the log
		return ;
	}

	port = ntohs(addr.sin_port);
	ip = std::string(reinterpret_cast<const char*>(&addr.sin_addr.s_addr), 4);
	client = new ClientHandler(fd, port, ip, _server, _factory);
	std::cout << "client connected: " << ip << ":" << port << std::endl;

	flags = fcntl(fd, F_GETFL, 0);
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
		std::cerr << "fcntl" << std::endl; // TODO: create exceptio
		return ;
	}

	_server->addHandler(fd, EPOLLIN, client);
}
