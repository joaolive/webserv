/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 20:41:50 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/02 21:26:47 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/model/HttpRequestParser.hpp"
#include "shared/Utils.hpp"

HttpRequestParser::HttpRequestParser() 
	: _state(STATE_REQUEST_LINE), _error_code(0), _content_length(0), _chunk_size(0), _bytes_to_read(0) {
	
	_dispatchTable[STATE_REQUEST_LINE] = &HttpRequestParser::_handleRequestLine;
	_dispatchTable[STATE_HEADERS]      = &HttpRequestParser::_handleHeaders;
	_dispatchTable[STATE_BODY]         = &HttpRequestParser::_handleBody;
	_dispatchTable[STATE_CHUNKED_SIZE] = &HttpRequestParser::_handleChunkedSize;
	_dispatchTable[STATE_CHUNKED_DATA] = &HttpRequestParser::_handleChunkedData;
}

HttpRequestParser::~HttpRequestParser() {}

void HttpRequestParser::feed(const std::vector<char>& chunk) {
	_buffer.insert(_buffer.end(), chunk.begin(), chunk.end());
	if (_state <= STATE_HEADERS && _buffer.size() > MAX_HEADER_SIZE) {
		_setError(431);
		return ;
	}
	ParseState last_state;
	do {
		last_state = _state;
		if (_state >= STATE_DONE)
			break;
		(this->*_dispatchTable[_state])();
	} while (last_state != _state && _state < STATE_DONE);
}

void HttpRequestParser::_handleRequestLine() {
	std::string line;
	if (_getLineFromBuffer(line)) {
		std::stringstream ss(line);
		ss >> _request.method >> _request.uri >> _request.version;
		if (_request.method.empty() || _request.uri.empty() || _request.version.empty())
			_setError(400);
		else
			_state = STATE_HEADERS;
	}
}

void HttpRequestParser::_handleHeaders() {
	std::string line;
	while (_getLineFromBuffer(line)) {
		if (line.empty()) { 
			_finalizeHeaders();
			return ;
		}
		size_t colon = line.find(':');
		if (colon == std::string::npos) {
			_setError(400); 
			return ;
		}
		std::string key = line.substr(0, colon);
		if (key.empty() || key.find_first_of(" \t") != std::string::npos) {
			_setError(400); 
			return ;
		}
		std::string value = line.substr(colon + 1);
		key = Utils::toLowerCase(key);
		value = Utils::trim(value);
		if (_request.headers.count(key))
			_request.headers[key] += ", " + value;
		else
			_request.headers[key] = value;
	}
}

void HttpRequestParser::_finalizeHeaders() {
	if (_request.headers.count("transfer-encoding")) {
		if (_request.headers["transfer-encoding"].find("chunked") != std::string::npos)
			_state = STATE_CHUNKED_SIZE;
		else
			_setError(501);
	} 
	else if (_request.headers.count("content-length")) {
		_content_length = std::strtoul(_request.headers["content-length"].c_str(), NULL, 10);
		_bytes_to_read = _content_length;
		_state = (_bytes_to_read > 0) ? STATE_BODY : STATE_DONE;
	} 
	else
		_state = STATE_DONE;
}

void HttpRequestParser::_handleBody() {
	size_t to_copy = std::min(_buffer.size(), _bytes_to_read);
	_request.body.insert(_request.body.end(), _buffer.begin(), _buffer.begin() + to_copy);
	_buffer.erase(_buffer.begin(), _buffer.begin() + to_copy);
	_bytes_to_read -= to_copy;
	if (_bytes_to_read == 0)
		_state = STATE_DONE;
}

void HttpRequestParser::_handleChunkedSize() {
	std::string line;
	if (_getLineFromBuffer(line)) {
		std::stringstream ss;
		ss << std::hex << line;
		if (!(ss >> _chunk_size)) {
			_setError(400);
			return ;
		}
		if (_chunk_size == 0)
			_state = STATE_DONE;
		else {
			_bytes_to_read = _chunk_size;
			_state = STATE_CHUNKED_DATA;
		}
	}
}

void HttpRequestParser::_handleChunkedData() {
	if (_buffer.size() >= _bytes_to_read + 2) {
		_request.body.insert(_request.body.end(), _buffer.begin(), _buffer.begin() + _bytes_to_read);
		_buffer.erase(_buffer.begin(), _buffer.begin() + _bytes_to_read + 2);
		_state = STATE_CHUNKED_SIZE;
	}
}

bool HttpRequestParser::_getLineFromBuffer(std::string& line) {
	std::vector<char>::iterator it = std::search(_buffer.begin(), _buffer.end(), "\r\n", "\r\n" + 2);
	if (it == _buffer.end())
		return (false);
	line.assign(_buffer.begin(), it);
	_buffer.erase(_buffer.begin(), it + 2);
	return (true);
}

void HttpRequestParser::_setError(int code) {
	_error_code = code;
	_state = STATE_ERROR;
}

void HttpRequestParser::reset() {
	_state = STATE_REQUEST_LINE;
	_request = HttpRequest();
	_buffer.clear();
	_error_code = 0;
	_content_length = 0;
	_chunk_size = 0;
	_bytes_to_read = 0;
}

bool HttpRequestParser::isDone() const {
	return (_state == STATE_DONE);
}

bool HttpRequestParser::hasError() const {
	return (_state == STATE_ERROR);
}

int HttpRequestParser::getErrorCode() const {
	return (_error_code);
}

const HttpRequest& HttpRequestParser::getRequest() const {
	return (_request);
}
