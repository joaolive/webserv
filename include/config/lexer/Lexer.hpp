/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 15:23:35 by joaolive          #+#    #+#             */
/*   Updated: 2026/04/27 17:07:01 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
//#include <stdexcept>
#include <sstream>

struct Token {
	std::string value;
	size_t line;

	Token(std::string v, size_t l) : value(v), line(l) {}
};

class Lexer {
	private:
		typedef void (Lexer::*LexerHandler)(const std::string& content, size_t& i);
		std::vector<Token> _tokens;
		std::string _currentToken;
		size_t _line;

		LexerHandler _dispatchTable[256];

		void initTable();
		void flushToken();
		void throwSyntaxError(const std::string& detail);

		void handleWord(const std::string& content, size_t& i);
		void handleNewline(const std::string& content, size_t& i);
		void handleQuote(const std::string& content, size_t& i);
		void handleComment(const std::string& content, size_t& i);
		void handleSpecial(const std::string& content, size_t& i);
	public:
		Lexer();

		std::vector<Token> tokenize(const std::string& content);
};