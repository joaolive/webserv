/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 18:21:59 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/01 15:34:56 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "config/lexer/Lexer.hpp"
#include "config/model/Config.hpp"
#include "config/parser/ServerParser.hpp"
#include "config/parser/LocationParser.hpp"

class Parser {
	private:
		static int parseServer(Config& config, const std::vector<Token>& tokens, size_t& i);
		static int parseLocation(ServerConfig& server, const std::vector<Token>& tokens, size_t& i);
		static int parseServerDirective(ServerConfig& server, const std::string& directive, const std::vector<Token>& tokens, size_t& i);
		static int parseLocationDirective(LocationConfig& location, const std::string& directive, const std::vector<Token>& tokens, size_t& i);
	public:
		static int parse(Config& config, const std::vector<Token>& tokens);
};
