/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseServer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 04:02:20 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/01 00:35:44 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parseServer.hpp"

int ParseServer::parseDirective(const std::string& directive, const std::vector<std::string>& args, ServerConfig& config) {
	static const std::map<std::string, DirectiveHandler> dispatchTable = initDispatchTable();
	std::map<std::string, DirectiveHandler>::const_iterator it = dispatchTable.find(directive);
	if (it != dispatchTable.end()) {
		DirectiveHandler handler = it->second;
		return handler(args, config);
	}
	std::cerr << "Parse error: Unknown directive '" << directive << "'\n";
	return (1);
}

std::map<std::string, ParseServer::DirectiveHandler> ParseServer::initDispatchTable() {
	std::map<std::string, DirectiveHandler> table;
	
	table["listen"] = &ParseServer::parseListen;
	table["host"] = &ParseServer::parseHost;
	table["root"] = &ParseServer::parseRoot;
	table["autoindex"] = &ParseServer::parseAutoindex;
	table["client_max_body_size"] = &ParseServer::parseClientMaxBodySize;
	table["index"] = &ParseServer::parseIndex;
	table["server_name"] = &ParseServer::parseServerName;
	table["error_page"] = &ParseServer::parseErrorPage;
	
	return (table);
}

int ParseServer::parseListen(const std::vector<std::string>& args, ServerConfig& config) {
	if (ParseUtils::validateArgsCount("listen", args, 1, 1))
		return (1);
	std::stringstream ss(args[0]);
	long port;
	ss >> port;
	if (ss.fail() || !ss.eof()) {
		std::cerr << "Parse error: Invalid port format: '" + args[0] + "'.\n";
		return (1);
	}
	if (port <= 0 || port > 65535) {
		std::cerr << "Parse error: Invalid port range: '" + args[0] + "'.\n";
		return (1);
	}
	config.setPort(static_cast<uint16_t>(port));
	return (0);
}

int ParseServer::parseHost(const std::vector<std::string>& args, ServerConfig& config) {
	if (ParseUtils::validateArgsCount("host", args, 1, 1))
		return (1);
	struct in_addr addr;
	int result = inet_pton(AF_INET, args[0].c_str(), &addr);
	if (result == 0) {
		std::cerr << "Parse error: Invalid 'host' address.\n";
		return (1);
	}
	if (result < 0) {
		std::cerr << "Parse error: 'host' address conversion failed.\n";
		return (1);
	}
	config.setHost(addr.s_addr);
	return (0);
}

int ParseServer::parseRoot(const std::vector<std::string>& args, ServerConfig& config) {
	if (ParseUtils::validateArgsCount("root", args, 1, 1))
		return (1);
	config.setRoot(args[0]); 
	return (0);
}

int ParseServer::parseAutoindex(const std::vector<std::string>& args, ServerConfig& config) {
	if (ParseUtils::validateArgsCount("autoindex", args, 1, 1))
		return (1);
	if (args[0] != "on" && args[0] != "off") {
		std::cerr << "Parse error: Invalid 'autoindex' argument.\n";
		return (1);
	}
	config.setAutoindex(args[0] == "on");
	return (0);
}

int ParseServer::parseClientMaxBodySize(const std::vector<std::string>& args, ServerConfig& config) {
	if (ParseUtils::validateArgsCount("client_max_body_size", args, 1, 1))
		return (1);
	uint64_t base_size = 0;
	char suffix = '\0';
	if (ParseUtils::extractSizeAndSuffix(args[0], base_size, suffix) != 0) {
		return (1);
	}
	if (suffix == '\0') {
		config.setClientMaxBodySize(base_size);
		return 0;
	}
	uint64_t final_size = 0;
	if (ParseUtils::applySizeMultiplier(base_size, suffix, final_size) != 0) {
		return 1;
	}
	config.setClientMaxBodySize(final_size);
	return (0);
}

int ParseServer::parseIndex(const std::vector<std::string>& args, ServerConfig& config) {
	if (ParseUtils::validateArgsCount("index", args, 1))
		return (1);
	for (size_t i = 0; i < args.size(); i++)
		config.addIndex(args[i]);
	return (0);
}


int ParseServer::parseServerName(const std::vector<std::string>& args, ServerConfig& config) {
	if (ParseUtils::validateArgsCount("server_name", args, 1))
		return (1);
	for (size_t i = 0; i < args.size(); i++)
		config.addServerName(args[i]);
	return (0);
}

int ParseServer::parseErrorPage(const std::vector<std::string>& args, ServerConfig& config) {
	if (ParseUtils::validateArgsCount("error_page", args, 2, 2))
		return (1);
	const std::string uri = args.back();
	for (size_t i = 0; i < args.size() - 1; i++) {
		int status_code;
		std::stringstream ss(args[i]);
		ss >> status_code;
		if (ss.fail() || !ss.eof()) {
			std::cerr << "Parse error: Invalid status code format '" << args[i] 
					  << "' in 'error_page'. Must be a pure integer.\n";
			 return (1);
		}
		if (!(status_code >= 300 && status_code <= 599)) {
			std::cerr << "Parse error: Status code " << status_code 
					  << " is out of valid range (300-599) in 'error_page'.\n";
			return (1);
		}
		config.addErrorPage(static_cast<uint16_t>(status_code), uri);
	}
	return (0);
}
