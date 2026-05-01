/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 18:44:13 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/01 01:00:35 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

int Parser::parse(Config& config, const std::vector<Token>& tokens) {
	size_t	i = 0;
	while (i < tokens.size()) {
		if (!(tokens[i].value == "server")) {
			std::cerr << "Parse error: Expected 'server' near line " << tokens[i].line << '\n';
			return (1);
		}
		i++;
		if (parseServer(config, tokens, i))
			return (1);
	}
	return (0);
}

int Parser::parseServer(Config& config, const std::vector<Token>& tokens, size_t& i) {
	if (i >= tokens.size() || tokens[i].value != "{") {
		std::cerr << "Parse error: Expected '{' after 'server' near line " << (i < tokens.size() ? tokens[i].line : tokens.back().line) << '\n';
		return (1);
	}
	i++;
	ServerConfig curr_server;
	while (i < tokens.size() && tokens[i].value != "}") {
		std::string directive = tokens[i].value;
		i++;
		if (directive == "location") {
			if (parseLocation(curr_server, tokens, i))
				return (1);
			continue ;
		}
		if (parseServerDirective(curr_server, directive, tokens, i))
			return (1);
	}
	if (i >= tokens.size() || tokens[i].value != "}") {
		std::cerr << "Parse error: Unexpected end of file, expected '}' for server block.\n";
		return (1);
	}
	i++;
	std::vector<LocationConfig>& locs = curr_server.getLocations();
	for (size_t j = 0; j < locs.size(); ++j)
		locs[j].inherit(curr_server);
	config.addServer(curr_server);
	return (0);
}

int Parser::parseServerDirective(ServerConfig& server, const std::string& directive, const std::vector<Token>& tokens, size_t& i) {
	std::vector<std::string> args;

	while (i < tokens.size() && tokens[i].value != ";" && tokens[i].value != "{" && tokens[i].value != "}")
		args.push_back(tokens[i++].value);
	if (i >= tokens.size() || tokens[i].value != ";") {
		size_t line = i < tokens.size() ? tokens[i].line : tokens.back().line;
		std::cerr << "Parse error: Expected ';' to terminate directive '" << directive << "' near line " << line << '\n';
		return (1);
	}
	i++;
	return (ParseServer::parseDirective(directive, args, server));
}

int Parser::parseLocation(ServerConfig& server, const std::vector<Token>& tokens, size_t& i) {
	if (i >= tokens.size()) {
		std::cerr << "Parse error: Expected path after 'location'.\n";
		return (1);
	}
	std::string path = tokens[i].value;
	i++;
	if (i >= tokens.size() || tokens[i].value != "{") {
		std::cerr << "Parse error: Expected '{' after location path '" << path << "'.\n";
		return (1);
	}
	i++;
	LocationConfig curr_location;
	curr_location.setPath(path);
	while (i < tokens.size() && tokens[i].value != "}") {
		std::string directive = tokens[i].value;
		i++;
		if (parseLocationDirective(curr_location, directive, tokens, i))
			return (1);
	}
	if (i >= tokens.size() || tokens[i].value != "}") {
		std::cerr << "Parse error: Unexpected end of file, expected '}' for location block.\n";
		return (1);
	}
	i++;
	server.addLocation(curr_location);
	return (0);
}

int Parser::parseLocationDirective(LocationConfig& location, const std::string& directive, const std::vector<Token>& tokens, size_t& i) {
	std::vector<std::string> args;

	while (i < tokens.size() && tokens[i].value != ";" && tokens[i].value != "{" && tokens[i].value != "}")
		args.push_back(tokens[i++].value);
	if (i >= tokens.size() || tokens[i].value != ";") {
		size_t line = i < tokens.size() ? tokens[i].line : tokens.back().line;
		std::cerr << "Parse error: Expected ';' to terminate directive '" << directive << "' near line " << line << '\n';
		return (1);
	}
	i++;
	return (ParseLocation::parseDirective(directive, args, location));
}