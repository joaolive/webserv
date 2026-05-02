/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 18:20:04 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/01 15:26:39 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include "config/model/ServerConfig.hpp"

class Config {
	private:
		std::vector<ServerConfig> _servers;
	public:
		Config();
		Config(const Config& rhs);
		Config& operator=(const Config& rhs);
		~Config();

		const std::vector<ServerConfig>& getServers() const;
		void addServer(const ServerConfig& server);
};
