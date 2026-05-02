/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 20:44:31 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/02 21:26:37 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shared/Utils.hpp"

std::string Utils::toLowerCase(const std::string& str) {
	std::string result = str;
	for (size_t i = 0; i < result.length(); ++i) {
		result[i] = std::tolower(result[i]);
	}
	return result;
}

std::string Utils::trim(const std::string& str) {
	size_t first = str.find_first_not_of(" \t");
	if (first == std::string::npos) return "";
	size_t last = str.find_last_not_of(" \t");
	return str.substr(first, (last - first + 1));
}