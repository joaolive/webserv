/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AcceptHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 18:12:36 by mhidani           #+#    #+#             */
/*   Updated: 2026/04/27 18:18:22 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "IEventHandler.hpp"
#include "IHttpServer.hpp"
#include "ClientHandler.hpp"
#include "Reactor.hpp"

#include <sys/epoll.h>

class AcceptHandler : public IEventHandler {
private:
	Reactor*		_eventLoop;
	IHttpServer*	_http;

public:
			AcceptHandler(Reactor* eventLoop, IHttpServer* http);
	virtual	~AcceptHandler(void);

	void	handleEvent(epoll_event& event);
};