/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseUtils.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 00:26:53 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/01 01:50:06 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdint.h>

class ParseUtils {
	public:
		static int extractSizeAndSuffix(const std::string& arg, uint64_t& size, char& suffix);
		static int applySizeMultiplier(uint64_t base_size, char suffix, uint64_t& final_size);
		static int validateArgsCount(const std::string& directive, const std::vector<std::string>& args, size_t min_expected, size_t max_expected = static_cast<size_t>(-1));
};