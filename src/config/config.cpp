/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 00:22:09 by joaolive          #+#    #+#             */
/*   Updated: 2026/04/30 19:36:30 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"

Config::Config() {}

Config::Config(const Config& rhs) {
	*this = rhs;
}

Config& Config::operator=(const Config& rhs) {
	if (this != &rhs)
		_servers = rhs._servers;
	return (*this);
}

Config::~Config() {}

// getters

const std::vector<ServerConfig>& Config::getServers() const {
	return _servers;
}

// adders

void Config::addServer(const ServerConfig& server) {
	_servers.push_back(server);
}

