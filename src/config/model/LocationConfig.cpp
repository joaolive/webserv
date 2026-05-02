/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 18:04:19 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/01 15:24:28 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/model/LocationConfig.hpp"
#include "config/model/ServerConfig.hpp"

LocationConfig::LocationConfig() 
	: _path(""), 
	  _alias(""), 
	  _root(""), 
	  _autoindex(false), 
	  _allow_methods(0), 
	  _client_max_body_size(0),
	  _return(std::make_pair(0, "")) {}

LocationConfig::LocationConfig(const LocationConfig& rhs) {
	*this = rhs;
}

LocationConfig& LocationConfig::operator=(const LocationConfig& rhs) {
	if (this != &rhs) {
		_path = rhs._path;
		_alias = rhs._alias;
		_root = rhs._root;
		_autoindex = rhs._autoindex;
		_allow_methods = rhs._allow_methods;
		_client_max_body_size = rhs._client_max_body_size;
		_index = rhs._index;
		_cgi_path = rhs._cgi_path;
		_cgi_ext = rhs._cgi_ext;
		_return = rhs._return;
		_error_pages = rhs._error_pages;
	}
	return (*this);
}

LocationConfig::~LocationConfig() {}

// getters

const std::string& LocationConfig::getPath() const {
	return (_path);
}

const std::string& LocationConfig::getAlias() const {
	return (_alias);
}

const std::string& LocationConfig::getRoot() const {
	return (_root);
}

bool LocationConfig::getAutoindex() const {
	return (_autoindex);
}

uint8_t LocationConfig::getAllowMethods() const {
	return (_allow_methods);
}

uint64_t LocationConfig::getClientMaxBodySize() const {
	return (_client_max_body_size);
}

const std::vector<std::string>& LocationConfig::getIndex() const {
	return (_index);
}

const std::vector<std::string>& LocationConfig::getCgiPath() const {
	return (_cgi_path);
}

const std::vector<std::string>& LocationConfig::getCgiExt() const {
	return (_cgi_ext);
}

const std::pair<uint16_t, std::string>& LocationConfig::getReturn() const {
	return (_return);
}

const std::map<uint16_t, std::string>& LocationConfig::getErrorPages() const {
	return (_error_pages);
}

// setters

void LocationConfig::setPath(const std::string& path) {
	_path = path;
}

void LocationConfig::setAlias(const std::string& alias) {
	_alias = alias;
}

void LocationConfig::setRoot(const std::string& root) {
	_root = root;
}

void LocationConfig::setAutoindex(bool autoindex) {
	_autoindex = autoindex;
}

void LocationConfig::setClientMaxBodySize(uint64_t clientMaxBodySize) {
	_client_max_body_size = clientMaxBodySize;
}

void LocationConfig::setReturn(uint16_t code, const std::string& url) {
	_return = std::make_pair(code, url); // C++98 way
}

// adders

void LocationConfig::addIndex(const std::string& index) {
	_index.push_back(index);
}

void LocationConfig::addCgiPath(const std::string& path) {
	_cgi_path.push_back(path);
}

void LocationConfig::addCgiExt(const std::string& ext) {
	_cgi_ext.push_back(ext);
}

void LocationConfig::addMethod(uint8_t method) {
	_allow_methods |= method;
}

void LocationConfig::addErrorPage(uint16_t key, const std::string& value) {
	_error_pages[key] = value;
}

// helpers

void LocationConfig::inherit(const ServerConfig& server) {
	if (_root.empty()) {
		_root = server.getRoot();
	}
	if (_index.empty()) {
		_index = server.getIndex();
	}
	if (_client_max_body_size == 0) {
		_client_max_body_size = server.getClientMaxBodySize();
	}
	_error_pages.insert(server.getErrorPages().begin(), server.getErrorPages().end());
}

bool LocationConfig::isMethodAllowed(uint8_t method) const {
	return ((_allow_methods & method) != 0);
}
