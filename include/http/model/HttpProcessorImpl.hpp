/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpProcessorImpl.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 16:21:17 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/02 23:55:15 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <iostream>
#include "contracts/IHttpProcessor.hpp"
#include "http/model/HttpRequestParser.hpp"
#include "config/model/ServerConfig.hpp"
#include "config/model/LocationConfig.hpp"

class HttpProcessorImpl : public IHttpProcessor {
	private:
		HttpProcessorImpl();
		HttpProcessorImpl(const HttpProcessorImpl& rhs);
		HttpProcessorImpl& operator=(const HttpProcessorImpl& rhs);

		const std::vector<ServerConfig>*	_servers;
		std::string							_client_ip;
		HttpRequestParser					_parser;
		const ServerConfig*					_resolved_server;
		const LocationConfig*				_resolved_location;

		bool								_is_ready;
		bool								_has_error;
		bool								_should_close;
		std::vector<char>					_response_buffer;

		void _buildErrorResponse(int status_code);
		void _buildSuccessResponse();
	public:
		HttpProcessorImpl(const std::vector<ServerConfig>* servers, const std::string& clientIP);
		~HttpProcessorImpl();

		void feedChunk(const char* data, size_t bytes_read);
		bool isReady() const;
		const std::vector<char>& buildResponse();
		void reset();
		bool shouldCloseConnection() const;
		bool hasError() const;
};