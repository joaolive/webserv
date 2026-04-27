/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 15:23:43 by joaolive          #+#    #+#             */
/*   Updated: 2026/04/27 17:37:39 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.hpp"

Lexer::Lexer() : _line(1) {
	initTable();
}

void Lexer::initTable() {
	for (int i = 0; i < 256; i++) {
		_dispatchTable[i] = &Lexer::handleWord;
	}
	_dispatchTable[static_cast<unsigned char>('\n')] = &Lexer::handleNewline;
	_dispatchTable[static_cast<unsigned char>('"')]  = &Lexer::handleQuote;
	_dispatchTable[static_cast<unsigned char>('#')]  = &Lexer::handleComment;

	_dispatchTable[static_cast<unsigned char>('{')]  = &Lexer::handleSpecial;
	_dispatchTable[static_cast<unsigned char>('}')]  = &Lexer::handleSpecial;
	_dispatchTable[static_cast<unsigned char>(';')]  = &Lexer::handleSpecial;
}

void Lexer::flushToken() {
	if (!_currentToken.empty()) {
		_tokens.push_back(Token(_currentToken, _line));
		_currentToken.clear();
	}
}

void Lexer::throwSyntaxError(const std::string& detail) {
	std::stringstream ss;
	ss << "Lexer error -> " << detail << " at line [" << _line << "]";
	throw (std::runtime_error(ss.str()));
}

void Lexer::handleWord(const std::string& content, size_t& i) {
	_currentToken += content[i++];
}

void Lexer::handleNewline(const std::string&, size_t& i) {
	flushToken();
	_line++;
	i++;
}

void Lexer::handleSpecial(const std::string& content, size_t& i) {
	flushToken();
	_tokens.push_back(Token(std::string(1, content[i]), _line));
	i++;
}

void Lexer::handleComment(const std::string& content, size_t& i) {
	flushToken();
	while (i < content.size() && content[i] != '\n')
		i++;
}

void Lexer::handleQuote(const std::string& content, size_t& i) {
	handleWord(content, i);
	while (i < content.size() && content[i] != '"') {
		if (content[i] == '\n')
			_line++;
		handleWord(content, i);
	}
	if (i >= content.size() || content[i] != '"')
		throwSyntaxError("unclosed string quote");
	handleWord(content, i);
	flushToken();
}

std::vector<Token> Lexer::tokenize(const std::string& content) {
	size_t i = 0;

	while (i < content.size()) {
		unsigned char c = static_cast<unsigned char>(content[i]);
		if (c == ' ' || c == '\t' || c == '\r' || c == '\v' || c == '\f') {
			flushToken();
			i++;
			continue ;
		}
		(this->*_dispatchTable[c])(content, i);
	}
	flushToken();
	return (_tokens);
}