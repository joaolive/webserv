/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseUtils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 00:28:50 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/01 01:57:46 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parseUtils.hpp"

int ParseUtils::extractSizeAndSuffix(const std::string& arg, uint64_t& size, char& suffix) {
	std::string str_suffix;
	std::stringstream ss(arg);

	ss >> size;
	if (ss.fail()) {
		std::cerr << "Parse error: 'client_max_body_size' is invalid or too large.\n";
		return (1);
	}
	ss >> str_suffix;
	if (str_suffix.empty()) {
		suffix = '\0';
		return (0);
	}
	if (str_suffix.size() > 1) {
		std::cerr << "Parse error: Invalid 'client_max_body_size' suffix.\n";
		return (1);
	}
	suffix = std::toupper(str_suffix[0]);
	return (0);
}

int ParseUtils::applySizeMultiplier(uint64_t base_size, char suffix, uint64_t& final_size) {
	uint64_t max_val = (uint64_t)-1;
	uint64_t multiplier = 1;
	const char* unitName = "";
	switch (suffix) {
		case 'K':
			multiplier = 1024;
			unitName = "Kilobytes";
			break;
		case 'M':
			multiplier = 1024 * 1024;
			unitName = "Megabytes";
			break;
		case 'G':
			multiplier = 1024 * 1024 * 1024;
			unitName = "Gigabytes";
			break;
		default:
			std::cerr << "Parse error: Unknown 'client_max_body_size' suffix.\n";
			return (1);
	}
	if (base_size > max_val / multiplier) {
		std::cerr << "Parse error: Overflow converting 'client_max_body_size' " 
				  << unitName << " to Bytes.\n";
		return (1);
	}
	final_size = base_size * multiplier;
	return (0);
}

int ParseUtils::validateArgsCount(const std::string& directive, const std::vector<std::string>& args, size_t min_expected, size_t max_expected) {
	if (args.size() >= min_expected && args.size() <= max_expected) {
		return (0);
	}
	const size_t NO_LIMIT = static_cast<size_t>(-1);
	bool is_exact = (min_expected == max_expected);
	bool is_at_least = (max_expected == NO_LIMIT);
	std::stringstream ss;
	ss << " and " << max_expected << " arguments.\n";
	std::string suffix = (is_exact || is_at_least) 
						 ? " argument(s).\n" 
						 : ss.str();
	std::cerr << "Parse error: '" << directive << "' requires "
			  << (is_exact ? "exactly " : is_at_least ? "at least " : "between ")
			  << min_expected 
			  << suffix;
	return (1);
}