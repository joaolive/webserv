/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 19:29:27 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/05 15:03:37 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <sstream>
#include "http/handler/RequestHandler.hpp"
// #include "http/handler/GetHandler.hpp"
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

void RequestHandler::execute() {
	static std::map<std::string, HandlerCreator> dispatch_table;
	if (dispatch_table.empty()) {
		// dispatch_table["GET"] = &createGet;
		// dispatch_table["POST"] = &createPost;
		// dispatch_table["PUT"] = &createPut;
		// dispatch_table["PATCH"] = &createPatch;
		// dispatch_table["DELETE"] = &createDelete;
	}
	IMethodHandler* handler = NULL;
	std::map<std::string, HandlerCreator>::iterator it = dispatch_table.find(_request.method);
	if (it != dispatch_table.end())
		handler = it->second();
	if (handler != NULL) {
		handler->handle(_request, _server, _location, _keep_alive, _response_buffer);
		delete (handler);
	} else
		_response_buffer = genErrorResponse(501, _server, _keep_alive);
}

const std::vector<char>& RequestHandler::getResponse() const {
	return (_response_buffer);
}

std::vector<char> RequestHandler::genErrorResponse(int status_code, const ServerConfig* server, bool keep_alive) {
	std::vector<char> buffer;
	std::stringstream ss;
	// TODO: implementar página de erro com base no ServerConfig.
	ss << "HTTP/1.1 " << status_code << " Error\r\n"
		<< "Content-Type: text/html\r\n";
	if (!keep_alive)
		ss << "Connection: close\r\n";
	ss << "\r\n"
		<< "<html><body><center><h1>" << status_code << "</h1></center><hr><center>Webserv/1.0</center></body></html>";
	std::string resp = ss.str();
	buffer.assign(resp.begin(), resp.end());
	return (buffer);
}


// TODO: Implementar os handlers abaixo.

// static IMethodHandler* createGet() {
// 	return (new GetHandler());
// }

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