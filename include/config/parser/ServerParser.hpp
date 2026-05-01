/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseServer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 03:48:38 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/01 00:33:52 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include "parseUtils.hpp"
#include "serverConfig.hpp"

class ParseServer {
	private:
		typedef int (*DirectiveHandler)(const std::vector<std::string>&, ServerConfig&);
		static std::map<std::string, DirectiveHandler> initDispatchTable();

		static int parseListen(const std::vector<std::string>& args, ServerConfig& config);
		static int parseHost(const std::vector<std::string>& args, ServerConfig& config);
		static int parseRoot(const std::vector<std::string>& args, ServerConfig& config);
		static int parseAutoindex(const std::vector<std::string>& args, ServerConfig& config);
		static int parseClientMaxBodySize(const std::vector<std::string>& args, ServerConfig& config);
		static int parseIndex(const std::vector<std::string>& args, ServerConfig& config);
		static int parseServerName(const std::vector<std::string>& args, ServerConfig& config);
		static int parseErrorPage(const std::vector<std::string>& args, ServerConfig& config);
	public:
		static int parseDirective(const std::string& directive, const std::vector<std::string>& args, ServerConfig& config);
};
