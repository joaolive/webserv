/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AcceptHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 18:12:36 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/03 10:23:50 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include "IEventHandler.hpp"
#include "ClientHandler.hpp"
#include "contracts/IHttpProcessorFactory.hpp"

class ServerEngine;

class AcceptHandler : public IEventHandler {
	private:
		int						_fd;
		ServerEngine*			_server;
		IHttpProcessorFactory&	_factory;
	protected:
		void prepareClient(int& fd);
	public:
		AcceptHandler(const int& fd, 
					  ServerEngine* sv, 
					  IHttpProcessorFactory& fc);
		virtual	~AcceptHandler(void);

		void event(epoll_event& event);
		void closeConnection(void);
		bool isTimeout(time_t now) const;
		void onTimeout(void);
};