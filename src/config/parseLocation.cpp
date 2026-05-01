/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseLocation.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 19:30:28 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/01 00:34:53 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parseLocation.hpp"

int ParseLocation::parseDirective(const std::string& directive, const std::vector<std::string>& args, LocationConfig& config) {
	static const std::map<std::string, DirectiveHandler> dispatchTable = initDispatchTable();
	std::map<std::string, DirectiveHandler>::const_iterator it = dispatchTable.find(directive);
	if (it != dispatchTable.end()) {
		DirectiveHandler handler = it->second;
		return handler(args, config);
	}
	std::cerr << "Parse error: Unknown directive '" << directive << "'\n";
	return (1);
}

std::map<std::string, ParseLocation::DirectiveHandler> ParseLocation::initDispatchTable() {
	std::map<std::string, DirectiveHandler> table;
	
	table["alias"] = &ParseLocation::parseAlias;
	table["root"] = &ParseLocation::parseRoot;
	table["autoindex"] = &ParseLocation::parseAutoindex;
	table["allow_methods"] = &ParseLocation::parseAllowMethods;
	table["client_max_body_size"] = &ParseLocation::parseClientMaxBodySize;
	table["index"] = &ParseLocation::parseIndex;
	table["cgi_path"] = &ParseLocation::parseCgiPath;
	table["cgi_ext"] = &ParseLocation::parseCgiExt;
	table["return"] = &ParseLocation::parseReturn;
	table["error_page"] = &ParseLocation::parseErrorPage;

	return (table);
}

int ParseLocation::parseAlias(const std::vector<std::string>& args, LocationConfig& config) {
	if (ParseUtils::validateArgsCount("alias", args, 1, 1))
		return (1);
	config.setAlias(args[0]);
	return (0);
}

int ParseLocation::parseRoot(const std::vector<std::string>& args, LocationConfig& config) {
	if (ParseUtils::validateArgsCount("root", args, 1, 1))
		return (1);
	config.setRoot(args[0]);
	return (0);
}

int ParseLocation::parseAutoindex(const std::vector<std::string>& args, LocationConfig& config) {
	if (ParseUtils::validateArgsCount("autoindex", args, 1, 1))
		return (1);
	if (args[0] != "on" && args[0] != "off") {
		std::cerr << "Parse error: Invalid 'autoindex' argument.\n";
		return (1);
	}
	config.setAutoindex(args[0] == "on");
	return (0);
}

int ParseLocation::parseAllowMethods(const std::vector<std::string>& args, LocationConfig& config) {
	if (ParseUtils::validateArgsCount("allow_methods", args, 1))
		return (1);
	static std::map<std::string, uint8_t> validMethods;
	if (validMethods.empty()) {
		validMethods["GET"] = METHOD_GET;
		validMethods["POST"] = METHOD_POST;
		validMethods["PUT"] = METHOD_PUT;
		validMethods["DELETE"] = METHOD_DELETE;
		validMethods["PATCH"] = METHOD_PATCH;
	}
	for (size_t i = 0; i < args.size(); i++) {
		std::map<std::string, uint8_t>::const_iterator it = validMethods.find(args[i]);
		if (it == validMethods.end()) {
			std::cerr << "Parse error: unknown or unsupported method '" << args[i] << "'.\n";
			return (1);
		}
		config.addMethod(it->second);
	}
	return (0);
}

int ParseLocation::parseClientMaxBodySize(const std::vector<std::string>& args, LocationConfig& config) {
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

int ParseLocation::parseIndex(const std::vector<std::string>& args, LocationConfig& config) {
	if (ParseUtils::validateArgsCount("index", args, 1))
		return (1);
	for (size_t i = 0; i < args.size(); i++)
		config.addIndex(args[i]);
	return (0);
}

int ParseLocation::parseCgiPath(const std::vector<std::string>& args, LocationConfig& config) {
	if (ParseUtils::validateArgsCount("cgi_path", args, 1))
		return (1);
	for (size_t i = 0; i < args.size(); i++)
		config.addCgiPath(args[i]);
	return (0);
}

int ParseLocation::parseCgiExt(const std::vector<std::string>& args, LocationConfig& config) {
	if (ParseUtils::validateArgsCount("cgi_ext", args, 1))
		return (1);
	for (size_t i = 0; i < args.size(); i++) {
		if (args[i].empty() || args[i][0] != '.') {
			std::cerr << "Parse error: CGI extension '" << args[i] << "' must start with a dot (.).\n";
			return (1);
		}
		config.addCgiExt(args[i]);
	}
	return (0);
}

int ParseLocation::parseReturn(const std::vector<std::string>& args, LocationConfig& config) {
	if (ParseUtils::validateArgsCount("return", args, 1, 2))
		return (1);
	uint16_t code = 302;
	std::string url = args[args.size() == 2];
	std::stringstream ss(args[0]);
	if (ss >> code && ss.eof()) {
		if (args.size() == 1)
			url = "";
	} else {
		if (args.size() == 2) {
			std::cerr << "Parse error: first argument of 'return' must be a numeric status code.\n";
			return (1);
		}
		code = 302;
	}
	if (!(code >= 100 && code <= 599)) {
		std::cerr << "Parse error: invalid HTTP status code '" << code << "'.\n";
		return (1);
	}
	config.setReturn(code, url);
	return (0);
}

int ParseLocation::parseErrorPage(const std::vector<std::string>& args, LocationConfig& config) {
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