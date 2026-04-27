/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reactor.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 09:38:46 by mhidani           #+#    #+#             */
/*   Updated: 2026/04/27 18:26:52 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "IServ.hpp"
#include "IConfig.hpp"
#include "IEventHandler.hpp"
#include "IHttpServer.hpp"
#include "AcceptHandler.hpp"

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <map>

#ifndef EVENT_LENGTH
# define EVENTS_LENGTH 1024
#endif

class Reactor : public IServ {
private:
	IConfig*						_config;
	IHttpServer*					_http;
	int								_servFd;
	int								_eventMonitorFd;
	std::map<int, IEventHandler*>	_handlers;

	int			createSocket(void) const;
	int			createMonitorHandlers(void) const;
	void		startEventLoop(void);

public:
				Reactor(IConfig* config, IHttpServer* http);
	virtual		~Reactor(void);

	IConfig*	config(void) const;
	int			servFd(void) const;
	int			eventMonitorFd(void) const;
	void		changeHandlerToWrite(const int& fd);
	void		addHandler(const int& fd, IEventHandler* handler);
	void		removeHandler(const int& fd);

	int			start(void);
};
