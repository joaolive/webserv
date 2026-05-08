/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerEngine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 19:21:08 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/07 20:05:42 by mhidani          ###   ########.fr       */
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
		const ServerConfig*				_config;
		IHttpProcessorFactory*	_httpProcFactory;
		std::map<int, IEventHandler*>	_handlers;
		int								_socketFd;
		int								_ioMonitorFd;
	protected:
		int createServer(void);
		int createIoMonitor(void);
	public:
		ServerEngine(const ServerConfig* config, 
					 IHttpProcessorFactory* httpProcFactory);
		virtual ~ServerEngine(void);

		void startEventLoop(void);

		const ServerConfig* getConfig(void) const;
		IHttpProcessorFactory* getHttpProcFactory(void) const;
		int getSocketFd(void) const;
		int getIoMonitorFd(void) const;
		IEventHandler* getHandler(const int& fd);
		void addHandler(const int& fd, const uint32_t& events, IEventHandler* hdl);
		void removeHandler(const int& fd);
		void changeHandlerState(const int& fd, const uint32_t& state);
};
