/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 19:29:27 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/08 18:21:56 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <fstream>
#include <sstream>
#include <memory>
#include "http/handler/RequestHandler.hpp"
#include "http/handler/GetHandler.hpp"
// #include "http/handler/PostHandler.hpp"
// #include "http/handler/PutHandler.hpp"
// #include "http/handler/PatchHandler.hpp"
// #include "http/handler/DeleteHandler.hpp"

RequestHandler::RequestHandler(const HttpRequest& request, const ServerConfig* server, const LocationConfig* location, bool keep_alive, const std::string& client_ip)
	: _request(request), 
		_server(server), 
		_location(location), 
		_keep_alive(keep_alive), 
		_client_ip(client_ip) {
}

RequestHandler::~RequestHandler() {}

// TODO: Implementar os handlers abaixo.

static IMethodHandler* createGet() {
	return (new GetHandler());
}

// static IMethodHandler* createPost() {
// 	return (new PostHandler());
// }

// static IMethodHandler* createPut() {
// 	return (new PutHandler());
// }

// static IMethodHandler* createPatch() {
// 	return (new PatchHandler());
// }

// static IMethodHandler* createDelete() {
// 	return (new DeleteHandler());
// }

void RequestHandler::execute() {
	static std::map<std::string, HandlerCreator> dispatch_table;
	if (dispatch_table.empty()) {
		dispatch_table["GET"] = &createGet;
		// dispatch_table["POST"] = &createPost;
		// dispatch_table["PUT"] = &createPut;
		// dispatch_table["PATCH"] = &createPatch;
		// dispatch_table["DELETE"] = &createDelete;
	}
	std::map<std::string, HandlerCreator>::iterator it = dispatch_table.find(_request.method);
	if (it != dispatch_table.end()) {
		std::auto_ptr<IMethodHandler> handler(it->second());
		if (handler.get() != NULL)
			handler->handle(_request, _server, _location, _keep_alive, _response_buffer);
	} else
		_response_buffer = genErrorResponse(501, _server, _keep_alive);
}

const std::vector<char>& RequestHandler::getResponse() const {
	return (_response_buffer);
}

std::vector<char> RequestHandler::genErrorResponse(int status_code, const ServerConfig* server, bool keep_alive) {
	std::vector<char> body;
	std::string path = (server != NULL) ? server->getErrorPage(status_code) : "";
	if (!path.empty()) {
		std::string full_path = server->getRoot() + path;
		std::ifstream file(full_path.c_str(), std::ios::binary | std::ios::ate);
		if (file.is_open()) {
			std::streamsize size = file.tellg();
			body.resize(size);
			file.seekg(0, std::ios::beg);
			if (!file.read(body.data(), size))
				body.clear();
		}
	}
	if (body.empty()) {
		std::stringstream ss;
		ss << "<html><body><center><h1>" << status_code << "</h1></center><hr><center>Webserv/1.0</center></body></html>";
		std::string fallback = ss.str();
		body.assign(fallback.begin(), fallback.end());
	}
	std::stringstream headers;
	headers << "HTTP/1.1 " << status_code << " Error\r\n"
			<< "Content-Type: text/html\r\n"
			<< "Content-Length: " << body.size() << "\r\n";
	if (!keep_alive)
		headers << "Connection: close\r\n";
	headers << "\r\n";
	std::string header_str = headers.str();
	std::vector<char> response(header_str.begin(), header_str.end());
	response.insert(response.end(), body.begin(), body.end());
	return (response);
}
