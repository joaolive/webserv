/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpProcessorImpl.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 22:50:16 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/02 23:54:25 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/model/HttpProcessorImpl.hpp"
#include "http/router/HttpRouter.hpp"
#include "http/handler/RequestHandler.hpp"

HttpProcessorImpl::HttpProcessorImpl(const std::vector<ServerConfig>* servers, const std::string& clientIP)
	: _servers(servers), 
	  _client_ip(clientIP),
	  _resolved_server(NULL), 
	  _resolved_location(NULL), 
	  _is_ready(false), 
	  _has_error(false), 
	  _should_close(false) {
}

HttpProcessorImpl::~HttpProcessorImpl() {}

void HttpProcessorImpl::feedChunk(const char* data, size_t bytes_read) {
	std::vector<char> chunk(data, data + bytes_read);
	_parser.feed(chunk);
	if (_parser.hasError()) {
		_buildErrorResponse(_parser.getErrorCode());
		_is_ready = true;
		_has_error = true;
		_should_close = true;
		return ;
	}
	if (_parser.isDone()) {
		const HttpRequest& req = _parser.getRequest();
		if (req.headers.count("connection") && req.headers.at("connection") == "close")
			_should_close = true;
		_resolved_server = HttpRouter::routeServer(*_servers, req);
		if (_resolved_server != NULL)
			_resolved_location = HttpRouter::routeLocation(*_resolved_server, req.uri);
		size_t max_body = _resolved_location ? _resolved_location->getClientMaxBodySize() :
							(_resolved_server ? _resolved_server->getClientMaxBodySize() : 0);
		(max_body > 0 && req.body.size() > max_body) ? _buildErrorResponse(413) : _buildSucessResponse(); 
		_is_ready = true;
	}
}

void HttpProcessorImpl::_buildSuccessResponse() {
	RequestHandler handler(
		_parser.getRequest(),
		_resolved_server,
		_resolved_location,
		!_should_close
		_client_ip
	);
	handler.execute();
	_response_buffer = handler.getResponse();
}

void HttpProcessorImpl::_buildErrorResponse(int status_code) {
	_response_buffer = RequestHandler::generateErrorResponse(
		status_code,
		_resolved_server,
		!_should_close
	);
}

bool HttpProcessorImpl::isReady() const {
	return (_is_ready);
}

const std::vector<char>& HttpProcessorImpl::buildResponse() {
	return _response_buffer;

}
bool HttpProcessorImpl::shouldCloseConnection() const {
	return _should_close;
}

bool HttpProcessorImpl::hasError() const {
	return _has_error;
}

void HttpProcessorImpl::reset() {
	_parser.reset();
	_resolved_server = NULL;
	_resolved_location = NULL;
	_is_ready = false;
	_has_error = false;
	_response_buffer.clear();
}