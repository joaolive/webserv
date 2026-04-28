/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 16:37:55 by mhidani           #+#    #+#             */
/*   Updated: 2026/04/28 10:53:45 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "IEventHandler.hpp"
#include "IHttpServer.hpp"
#include "ClientHandler.hpp"
#include "Reactor.hpp"

#include <errno.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#ifndef READ_BUFFER_SIZE
# define READ_BUFFER_SIZE 1024
#endif

enum ClientHandlerState {
	READING,
	WRITING,
	CLOSED
};

class Reactor;

class ClientHandler : public IEventHandler {
private:
	int				_state;
	int				_fd;
	Reactor*		_eventLoop;
	IHttpServer*	_http;
	std::string		_readBuffer;
	std::string		_writeBuffer;

	void	closeConnection(void);

public:
			ClientHandler(const int& fd, Reactor* eventLoop, IHttpServer* http);
	virtual	~ClientHandler(void);

	void	handleEvent(epoll_event& event);
	int		read(void);
	int		write(void);
};
