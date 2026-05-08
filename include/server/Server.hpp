/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 19:13:16 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/07 19:57:28 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include "config/io/ReadFile.hpp"
#include "config/lexer/Lexer.hpp"
#include "config/parser/Parser.hpp"
#include "config/model/Config.hpp"
#include "config/model/ServerConfig.hpp"
#include "contracts/IHttpProcessorFactory.hpp"
#include "http/model/HttpProcessorFactoryImpl.hpp"
#include "infra/engine/ServerEngine.hpp"

class Server {
	protected:
		static void startServerEngine(const ServerConfig* conf, 
									  IHttpProcessorFactory* http);
	public:
		static int start(const std::string& fileConf);
};
