/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 18:17:13 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/06 12:30:02 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/model/ServerConfig.hpp"

ServerConfig::ServerConfig()
	: _port(0),
	  _host(0),
	  _root(""),
	  _autoindex(false),
	  _client_max_body_size(0) {}

ServerConfig::ServerConfig(const ServerConfig& rhs) {
	*this = rhs;
}

ServerConfig& ServerConfig::operator=(const ServerConfig& rhs) {
	if (this != &rhs) {
		_port = rhs._port;
		_host = rhs._host;
		_root = rhs._root;
		_autoindex = rhs._autoindex;
		_client_max_body_size = rhs._client_max_body_size;
		_index = rhs._index;
		_server_names = rhs._server_names;
		_error_pages = rhs._error_pages;
		_locations = rhs._locations;
	}
	return (*this);
}

ServerConfig::~ServerConfig() {};

// getters

uint16_t ServerConfig::getPort() const {
	return (_port);
}

in_addr_t ServerConfig::getHost() const {
	return (_host);
}

const std::string& ServerConfig::getRoot() const {
	return (_root);
}

bool ServerConfig::getAutoindex() const {
	return (_autoindex);
}

uint64_t ServerConfig::getClientMaxBodySize() const {
	return (_client_max_body_size);
}

const std::vector<std::string>& ServerConfig::getIndex() const {
	return (_index);
}

const std::vector<std::string>& ServerConfig::getServerNames() const {
	return (_server_names);
}

const std::map<uint16_t, std::string>& ServerConfig::getErrorPages() const {
	return (_error_pages);
}

std::string ServerConfig::getErrorPage(uint16_t code) const {
	std::map<uint16_t, std::string>::const_iterator it = _error_pages.find(code);
	if (it != _error_pages.end()) {
		return (it->second);
	}
	return ("");
}

const std::vector<LocationConfig>& ServerConfig::getLocations() const {
	return (_locations);
}

std::vector<LocationConfig>& ServerConfig::getLocations() {
	return (_locations);
}

// setters

void ServerConfig::setPort(uint16_t port) {
	_port = port;
}

void ServerConfig::setHost(in_addr_t host) {
	_host = host;
}

void ServerConfig::setRoot(const std::string& root) {
	_root = root;
}

void ServerConfig::setAutoindex(bool autoindex) {
	_autoindex = autoindex;
}

void ServerConfig::setClientMaxBodySize(uint64_t clientMaxBodySize) {
	_client_max_body_size = clientMaxBodySize;
}

// adders

void ServerConfig::addServerName(const std::string& server_name) {
	_server_names.push_back(server_name);
}

void ServerConfig::addIndex(const std::string& index) {
	_index.push_back(index);
}

void ServerConfig::addErrorPage(uint16_t key, const std::string& value) {
	_error_pages[key] = value;
}

void ServerConfig::addLocation(const LocationConfig& location) {
	_locations.push_back(location);
}