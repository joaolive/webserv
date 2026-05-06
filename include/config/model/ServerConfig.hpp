/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 18:05:38 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/06 12:28:26 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>
#include <arpa/inet.h>
#include "config/model/LocationConfig.hpp"

class ServerConfig {
	private:
		uint16_t						_port;
		in_addr_t						_host;
		std::string						_root;
		bool							_autoindex;
		uint64_t						_client_max_body_size;
		std::vector<std::string>		_index;
		std::vector<std::string>		_server_names;
		std::map<uint16_t, std::string>	_error_pages;
		std::vector<LocationConfig>		_locations;
	public:
		ServerConfig();
		ServerConfig(const ServerConfig& rhs);
		ServerConfig& operator=(const ServerConfig& rhs);
		~ServerConfig();

		uint16_t getPort() const;
		in_addr_t getHost() const;
		const std::string& getRoot() const;
		bool getAutoindex() const;
		uint64_t getClientMaxBodySize() const;
		const std::vector<std::string>& getIndex() const;
		const std::vector<std::string>& getServerNames() const;
		const std::map<uint16_t, std::string>& getErrorPages() const;
		std::string getErrorPage(uint16_t code) const;
		const std::vector<LocationConfig>& getLocations() const;
		std::vector<LocationConfig>& getLocations();
		
		void setPort(uint16_t port);
		void setHost(in_addr_t host);
		void setRoot(const std::string& root);
		void setAutoindex(bool autoindex);
		void setClientMaxBodySize(uint64_t clientMaxBodySize);

		void addServerName(const std::string& server_name);
		void addIndex(const std::string& index);
		void addErrorPage(uint16_t key, const std::string& value);
		void addLocation(const LocationConfig& location);
};
