/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestParser.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 20:02:59 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/02 21:15:57 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <sstream>

struct HttpRequest {
	std::string method;
	std::string uri;
	std::string version;
	std::map<std::string, std::string> headers;
	std::vector<char> body;
};

enum ParseState {
	STATE_REQUEST_LINE,
	STATE_HEADERS,
	STATE_BODY,
	STATE_CHUNKED_SIZE,
	STATE_CHUNKED_DATA,
	STATE_DONE,
	STATE_ERROR
};

class HttpRequestParser {
	private:
		typedef void (HttpRequestParser::*Handler)();

		static const size_t	MAX_HEADER_SIZE = 8192;

		ParseState			_state;
		Handler				_dispatchTable[5];
		HttpRequest			_request;
		std::vector<char>	_buffer;
		int					_error_code;
		size_t				_content_length;
		size_t				_chunk_size;
		size_t				_bytes_to_read;

		void _handleRequestLine();
		void _handleHeaders();
		void _handleBody();
		void _handleChunkedSize();
		void _handleChunkedData();

		bool _getLineFromBuffer(std::string& line);
		void _finalizeHeaders();
		void _setError(int code);

		HttpRequestParser(const HttpRequestParser&);
		HttpRequestParser& operator=(const HttpRequestParser&);
	public:
		HttpRequestParser();
		~HttpRequestParser();

		void feed(const std::vector<char>& chunk);
		
		bool isDone() const;
		bool hasError() const;
		void reset();
		const HttpRequest& getRequest() const;
		int getErrorCode() const;
};
