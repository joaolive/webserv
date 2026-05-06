/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerEngine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 19:21:08 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/06 14:50:19 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "IEventHandler.hpp"
#include "AcceptHandler.hpp"
#include "config/model/ServerConfig.hpp"
#include "contracts/IHttpProcessorFactory.hpp"
#include "infra/exception/SocketException.hpp"
#include "infra/exception/IoMonitorException.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <stdexcept>
#include <map>
#include <vector>

#ifndef IO_MONITOR_SIZE
# define IO_MONITOR_SIZE 2048
#endif

#ifndef STREAMING_BUFFER_SIZE
# define STREAMING_BUFFER_SIZE 8
#endif

#ifndef TIMEOUT
# define TIMEOUT 30
#endif

class ServerEngine {
	private:
		int								_socketFd;
		int								_ioMonitorFd;
		std::map<int, IEventHandler*>	_handlers;
		ServerConfig*					_config;
	protected:
		int createServer(void);
		int createIoMonitor(void);
		void startEventLoop(void);
	public:
		ServerEngine(ServerConfig* config);
		virtual ~ServerEngine(void);

		static int start(ServerConfig* config, IHttpProcessorFactory& factory);

		int getSocketFd(void) const;
		int getIoMonitorFd(void) const;
		ServerConfig* getConfig(void) const;
		IEventHandler* getHandler(const int& fd);
		void addHandler(const int& fd, const uint32_t& events, IEventHandler* hdl);
		void removeHandler(const int& fd);
		void changeHandlerState(const int& fd, const uint32_t& state);
};
