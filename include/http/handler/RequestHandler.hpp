/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 19:31:52 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/08 18:59:09 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>
#include "http/model/HttpRequestParser.hpp"
#include "config/model/ServerConfig.hpp"
#include "config/model/LocationConfig.hpp"
#include "contracts/IMethodHandler.hpp"

class RequestHandler {
	private:
		typedef IMethodHandler* (*HandlerCreator)();

		const HttpRequest&		_request;
		const ServerConfig*		_server;
		const LocationConfig*	_location;
		bool					_keep_alive;
		std::string				_client_ip;
		std::vector<char>		_response_buffer;

		RequestHandler();
		RequestHandler(const RequestHandler& rhs);
		RequestHandler& operator=(const RequestHandler& rhs);
		bool redirect(void);
	public:
		RequestHandler(const HttpRequest& request,
						const ServerConfig* server,
						const LocationConfig* Location,
						bool keep_alive,
						const std::string& client_ip);
		~RequestHandler();

		void execute();
		const std::vector<char>& getResponse() const;
		static std::vector<char> genErrorResponse(int status_code,
												const ServerConfig* server, bool keep_alive);
};