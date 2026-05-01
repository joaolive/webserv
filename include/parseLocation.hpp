/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseLocation.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 19:23:22 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/01 00:33:21 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "parseUtils.hpp"
#include "locationConfig.hpp"

class ParseLocation {
	private:
		typedef int (*DirectiveHandler)(const std::vector<std::string>&, LocationConfig&);
		static std::map<std::string, DirectiveHandler> initDispatchTable();

		static int parseAlias(const std::vector<std::string>& args, LocationConfig& config);
		static int parseRoot(const std::vector<std::string>& args, LocationConfig& config);
		static int parseAutoindex(const std::vector<std::string>& args, LocationConfig& config);
		static int parseAllowMethods(const std::vector<std::string>& args, LocationConfig& config);
		static int parseClientMaxBodySize(const std::vector<std::string>& args, LocationConfig& config);
		static int parseIndex(const std::vector<std::string>& args, LocationConfig& config);
		static int parseCgiPath(const std::vector<std::string>& args, LocationConfig& config);
		static int parseCgiExt(const std::vector<std::string>& args, LocationConfig& config);
		static int parseReturn(const std::vector<std::string>& args, LocationConfig& config);
		static int parseErrorPage(const std::vector<std::string>& args, LocationConfig& config);
	public:
		static int parseDirective(const std::string& directive, const std::vector<std::string>& args, LocationConfig& config);
};
