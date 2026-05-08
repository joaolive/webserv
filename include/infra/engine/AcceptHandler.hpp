/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AcceptHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 18:12:36 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/08 15:18:28 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include "infra/engine/IEventHandler.hpp"
#include "contracts/IHttpProcessorFactory.hpp"

class ServerEngine;
class ClientHandler;

class AcceptHandler : public IEventHandler {
	private:
		ServerEngine*	_serverEngine;
		int				_fd;
		Stage			_stage;
	protected:
		ClientHandler* prepareClient(const int& socketFd);
	public:
		AcceptHandler(ServerEngine* serverEngine);
		virtual	~AcceptHandler(void);

		void event(epoll_event&);
		void closeConnection(void);
		bool isTimeout(time_t) const;
		void onTimeout(void);
		Stage stage(void) const;
};