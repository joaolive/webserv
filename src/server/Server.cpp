/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 19:18:28 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/07 20:32:08 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"

int Server::start(const std::string& fileConf) {
	try {
		std::string content = ReadFile::readFile(fileConf);
		Lexer lexer;
		Config config;
		std::vector<Token> tokens = lexer.tokenize(content);
		IHttpProcessorFactory* httpProcFactory = NULL;

		if (Parser::parse(config, tokens) != 0)
			throw std::runtime_error("the parser could not be executed");

		const std::vector<ServerConfig>& configs = config.getServers();
		if (configs.empty())
			throw std::runtime_error("there is no \"server\" block in the configuration file");
		
		httpProcFactory = new HttpProcessorFactoryImpl(config);
		for (size_t i = 0; i < configs.size(); i++)
			startServerEngine(&configs[i], httpProcFactory);

		delete httpProcFactory;

	} catch (const std::exception& e) {
		std::cerr << "error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}

void Server::startServerEngine(const ServerConfig* conf, 
							   IHttpProcessorFactory* httpProcFactory) {
	ServerEngine serverEngine(conf, httpProcFactory);
	AcceptHandler* acceptHdl = new AcceptHandler(&serverEngine);

	std::cout << "running server " << conf->getHost() << ":" << conf->getPort() << std::endl;
	serverEngine.addHandler(serverEngine.getSocketFd(), EPOLLIN, acceptHdl);
	serverEngine.startEventLoop();
}