/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 17:57:52 by joaolive          #+#    #+#             */
/*   Updated: 2026/04/30 20:51:15 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <cstdint>
#include <map>
#include <vector>

enum HttpMethod {
	METHOD_NONE		= 0,
	METHOD_GET		= 1 << 0,
	METHOD_POST		= 1 << 1,
	METHOD_PUT		= 1 << 2,
	METHOD_DELETE	= 1 << 3,
	METHOD_PATCH	= 1 << 4
};

class LocationConfig {
	private:
		std::string							_path;
		std::string							_alias;
		std::string							_root;
		bool								_autoindex;
		uint8_t								_allow_methods;
		uint64_t							_client_max_body_size;
		std::vector<std::string>			_index;
		std::vector<std::string>			_cgi_path;
		std::vector<std::string>			_cgi_ext;
		std::pair<uint16_t, std::string>	_return;
		std::map<uint16_t, std::string>		_error_pages;
	public:
		LocationConfig();
		LocationConfig(const LocationConfig& rhs);
		LocationConfig& operator=(const LocationConfig& rhs);
		~LocationConfig();

		const std::string& getPath() const;
		const std::string& getAlias() const;
		const std::string& getRoot() const;
		bool getAutoindex() const;
		uint8_t getAllowMethods() const;
		uint64_t getClientMaxBodySize() const;
		const std::vector<std::string>& getIndex() const;
		const std::vector<std::string>& getCgiPath() const;
		const std::vector<std::string>& getCgiExt() const;
		const std::pair<uint16_t, std::string>& getReturn() const;
		const std::map<uint16_t, std::string>& getErrorPages() const;

		void setPath(const std::string& path);
		void setAlias(const std::string& alias);
		void setRoot(const std::string& root);
		void setAutoindex(bool autoindex);
		void setClientMaxBodySize(uint64_t clientMaxBodySize);
		void setReturn(uint16_t code, const std::string& url);

		void addIndex(const std::string& index);
		void addCgiPath(const std::string& path);
		void addCgiExt(const std::string& ext);
		void addMethod(uint8_t method);
		void addErrorPage(uint16_t key, const std::string& value);

		bool isMethodAllowed(uint8_t method) const;
};
