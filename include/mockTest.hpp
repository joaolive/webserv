/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mockTest.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 15:11:04 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/06 15:37:14 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <stdint.h>
#include "contracts/IHttpProcessor.hpp"
#include "contracts/IHttpProcessorFactory.hpp"

class MockHttpProcessor : public IHttpProcessor {
private:
	bool _ready;
	bool _error;
	std::string _request;
	std::string _response;

public:
	MockHttpProcessor()
		: _ready(false),
		  _error(false),
		  _request(),
		  _response("HTTP/1.1 200 OK\r\nContent-Length: 2\r\nConnection: close\r\n\r\nOK")
	{}

	virtual ~MockHttpProcessor() {}

	virtual void feedChunk(const char* data, size_t bytes_read) {
		(void)data;
		(void)bytes_read;
		_ready = true;
	}

	virtual bool isReady() const {
		return _ready;
	}

	virtual std::string buildResponse() {
		_ready = false;
		return _response;
	}

	virtual void reset() {
		_ready = false;
		_request.clear();
	}

	virtual bool shouldCloseConnection() const {
		return true;
	}

	virtual bool hasError() const {
		return _error;
	}
};

class MockHttpProcessorFactory : public IHttpProcessorFactory {
public:
	virtual ~MockHttpProcessorFactory() {}

	virtual IHttpProcessor* createProcessor(const std::string& client_ip,
								uint16_t server_port) {
		(void)client_ip;
		(void)server_port;
		return new MockHttpProcessor();
	}
};



