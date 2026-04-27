/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readFile.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 15:11:55 by joaolive          #+#    #+#             */
/*   Updated: 2026/04/27 15:37:08 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <sys/stat.h>

class ReadFile {
	private:
		static void checkPath(const std::string& filename);
		static void checkStream(std::ifstream& file);
		static std::string readStream(std::ifstream& file);
		static void checkContent(const std::string& content);
	public:
		static std::string readFile(const std::string& filename);
};