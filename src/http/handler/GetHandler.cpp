/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 13:14:02 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/06 14:16:29 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/handler/GetHandler.hpp"
#include "http/handler/RequestHandler.hpp"
#include <sys/stat.h>
#include <fstream>
#include <sstream>

GetHandler::GetHandler() {}

GetHandler::~GetHandler() {}

void GetHandler::handle(const HttpRequest& request, const ServerConfig* server, const LocationConfig* location, bool keep_alive, std::vector<char>& response_buffer) {
	
	std::string physical_path = _resolvePath(request, server, location);
	struct stat file_stat;
	if (stat(physical_path.c_str(), &file_stat) == 0) {
		if (S_ISDIR(file_stat.st_mode))
			response_buffer = RequestHandler::genErrorResponse(403, server, keep_alive);
		else if (S_ISREG(file_stat.st_mode))
			_serveStaticFile(physical_path, file_stat.st_size, server, keep_alive, response_buffer);
		else
			response_buffer = RequestHandler::genErrorResponse(403, server, keep_alive);
	} else
		response_buffer = RequestHandler::genErrorResponse(404, server, keep_alive);
}

void GetHandler::_serveStaticFile(const std::string& path, off_t file_size, const ServerConfig* server, bool keep_alive, std::vector<char>& response_buffer) {
	std::ifstream file(path.c_str(), std::ios::binary);
	if (!file.is_open()) {
		response_buffer = RequestHandler::genErrorResponse(403, server, keep_alive);
		return ;
	}
	std::vector<char> file_content(file_size);
	if (file_size > 0 && !file.read(&file_content[0], file_size)) {
		response_buffer = RequestHandler::genErrorResponse(500, server, keep_alive);
		return ;
	}
	std::string mime_type = _getMimeType(path);
	std::stringstream headers;
	headers << "HTTP/1.1 200 OK\r\n"
			<< "Content-Type: " << mime_type << "\r\n"
			<< "Content-Length: " << file_size << "\r\n";
	if (!keep_alive)
		headers << "Connection: close\r\n";
	headers << "\r\n";
	std::string header_str = headers.str();
	response_buffer.reserve(header_str.size() + file_size);
	response_buffer.assign(header_str.begin(), header_str.end());
	response_buffer.insert(response_buffer.end(), file_content.begin(), file_content.end());
}

std::string GetHandler::_resolvePath(const HttpRequest& request, const ServerConfig* server, const LocationConfig* location) {
	std::string root = "";
	if (location != NULL && !location->getRoot().empty())
		root = location->getRoot();
	else if (server != NULL && !server->getRoot().empty())
		root = server->getRoot();
	else
		root = "/var/www/html";
	if (!root.empty() && root[root.length() - 1] == '/')
		root.erase(root.length() - 1);
	return (root + request.uri);
}

std::string GetHandler::_getMimeType(const std::string& path) {
	size_t dot_pos = path.find_last_of('.');
	if (dot_pos == std::string::npos)
		return ("application/octet-stream");
	std::string ext = path.substr(dot_pos);
	if (ext == ".html" || ext == ".htm") return ("text/html");
	if (ext == ".css")                   return ("text/css");
	if (ext == ".js")                    return ("application/javascript");
	if (ext == ".png")                   return ("image/png");
	if (ext == ".jpg" || ext == ".jpeg") return ("image/jpeg");
	if (ext == ".gif")                   return ("image/gif");
	if (ext == ".ico")                   return ("image/x-icon");
	if (ext == ".txt")                   return ("text/plain");
	if (ext == ".pdf")                   return ("application/pdf");
	if (ext == ".json")                  return ("application/json");
	return ("application/octet-stream");
}