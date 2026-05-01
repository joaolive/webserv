/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReadFile.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 09:07:55 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/01 15:23:56 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/io/ReadFile.hpp"

void ReadFile::checkPath(const std::string& filename) {
	struct stat st;
	
	if (stat(filename.c_str(), &st) != 0)
		throw (std::runtime_error("Error: file does not exist"));
	if (!S_ISREG(st.st_mode))
		throw (std::runtime_error("Error: not a regular file"));
}

void ReadFile::checkStream(std::ifstream& file) {
	if (!file.is_open())
		throw (std::runtime_error("Error: cannot open file"));
}

std::string ReadFile::readStream(std::ifstream& file) {
	std::string content((std::istreambuf_iterator<char>(file)),
											std::istreambuf_iterator<char>());
	if (file.bad())
		throw (std::runtime_error("Error: read failed"));
	return (content);
}

void ReadFile::checkContent(const std::string& content) {
	if (content.empty())
		throw (std::runtime_error("Error: file is empty"));
}

std::string ReadFile::readFile(const std::string& filename) {
	checkPath(filename);
	std::ifstream file(filename.c_str());
	checkStream(file);
	std::string content = readStream(file);
	checkContent(content);
	return (content);
}