/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpProcessorFactoryImpl.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 15:51:31 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/02 17:55:19 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/model/HttpProcessorFactoryImpl.hpp"
#include "http/model/HttpProcessorImpl.hpp"

HttpProcessorFactoryImpl::HttpProcessorFactoryImpl(const Config& config) {
	const std::vector<ServerConfig>& servers = config.getServers();
	for (size_t i = 0; i < servers.size(); i++)
		_dispatchTable[servers[i].getPort()].push_back(servers[i]);
}

HttpProcessorFactoryImpl::~HttpProcessorFactoryImpl() {}

IHttpProcessor* HttpProcessorFactoryImpl::createProcessor(const std::string& client_ip, uint16_t server_port) {
	const std::vector<ServerConfig>* matchedServers = &_dispatchTable[server_port];
	if (matchedServers->empty())
		return (NULL);
	return (new HttpProcessorImpl(matchedServers, client_ip));
}