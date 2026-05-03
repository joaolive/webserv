/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 20:00:06 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/03 09:19:44 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "IEventHandler.hpp"
#include "contracts/IHttpProcessor.hpp"
#include "contracts/IHttpProcessorFactory.hpp"

class ServerEngine;

class ClientHandler : public IEventHandler {
	private:
		int				_fd;
		uint32_t		_port;
		std::string		_ip;
		ServerEngine*	_server;
		IHttpProcessor* _processor;
		std::string		_writeBuffer;
		size_t			_writeOffset;
		bool			_closeAfterWrite;
		unsigned long	_timeout;
	protected:
		void onReading(void);
		void onWriting(void);
		void prepareResponse(void);
		void closeConnection(void);
	public:
		ClientHandler(int fd, 
					  uint32_t port, 
					  std::string ip, 
					  ServerEngine* sv, 
					  IHttpProcessorFactory& fc);
		virtual ~ClientHandler(void);

		void event(epoll_event& event);
};
