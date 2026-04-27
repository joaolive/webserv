/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AcceptHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 13:54:52 by mhidani           #+#    #+#             */
/*   Updated: 2026/04/27 18:20:33 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AcceptHandler.hpp"

AcceptHandler::AcceptHandler(Reactor* eventLoop, IHttpServer* http) {
	_eventLoop = eventLoop;
	_http = http;
}

AcceptHandler::~AcceptHandler(void) {
}

void AcceptHandler::handleEvent(epoll_event& event) {
	sockaddr_in		addr;
	ClientHandler*	hanlder = NULL;
	socklen_t		addrLen = sizeof(addr);
	int				fd = accept(_eventLoop->servFd(), 
								reinterpret_cast<sockaddr*>(&addr), 
								&addrLen);

	if (fd < 0) {
		std::cerr << "accept" << std::endl; // TODO: insert in the log
		return;
	}
	
	fcntl(fd, F_SETFL, O_NONBLOCK);
	hanlder = new ClientHandler(fd, _eventLoop, _http);
	_eventLoop->addHandler(fd, hanlder);
	std::cout << "client (" << fd << ") connected." << std::endl; // TODO : insert in the log
}
