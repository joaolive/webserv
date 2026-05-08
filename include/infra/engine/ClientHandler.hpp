/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vscode <vscode@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 20:00:06 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/08 12:12:16 by vscode           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/time.h>
#include "infra/engine/IEventHandler.hpp"
#include "contracts/IHttpProcessor.hpp"
#include "contracts/IHttpProcessorFactory.hpp"

class ServerEngine;

class ClientHandler : public IEventHandler {
	private:
		int				_fd;
		uint32_t		_port;
		std::string		_ip;
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
	public:
		ClientHandler(const int fd, const char* ip, uint16_t port, 
					  ServerEngine* serverEngine);
		virtual ~ClientHandler(void);

		void event(epoll_event& event);
		void closeConnection(void);
		bool isTimeout(time_t now) const;
		void onTimeout(void);
		Stage stage(void) const;
};
