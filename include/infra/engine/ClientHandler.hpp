/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 20:00:06 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/11 21:56:25 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/time.h>
#include <vector>
#include "infra/engine/IEventHandler.hpp"
#include "contracts/IHttpProcessor.hpp"
#include "contracts/IHttpProcessorFactory.hpp"

class ServerEngine;

class ClientHandler : public IEventHandler {
	private:
		int				_fd;
		std::string		_ip;
		uint32_t		_port;
		ServerEngine*	_serverEngine;
		IHttpProcessor* _httpProcessor;
		std::string		_writeBuffer;
		size_t			_writeOffset;
		bool			_closeAfterWrite;
		time_t			_lastActivity;
		Stage			_stage;
	protected:
		void onReading(void);
		void onWriting(void);
		void prepareResponse(void);
		void finishWriting(void);
	public:
		ClientHandler(const int fd, const std::string ip, uint16_t port, 
					  ServerEngine* serverEngine);
		virtual ~ClientHandler(void);

		void event(epoll_event& event);
		void closeConnection(const Stage& stage);
		bool isTimeout(time_t now) const;
		void onTimeout(void);
		Stage stage(void) const;

		int getFd(void) const;
		std::string getIp(void) const;
		uint32_t getPort(void) const;
};
