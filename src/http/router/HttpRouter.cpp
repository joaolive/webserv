/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRouter.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 21:39:49 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/02 22:11:43 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/router/HttpRouter.hpp"

const ServerConfig* HttpRouter::routeServer(const std::vector<ServerConfig>& servers, const HttpRequest& request) {
	if (servers.empty())
		return (NULL);
	std::string host_header = "";
	if (request.headers.count("host")) {
		host_header = request.headers.find("host")->second;
		size_t colon_pos = host_header.find(':');
		if (colon_pos != std::string::npos)
			host_header = host_header.substr(0, colon_pos);
	}
	for (std::vector<ServerConfig>::const_iterator it = servers.begin(); it != servers.end(); it++) {
		const std::vector<std::string>& servers_names = it->getServerNames();
		for (size_t i = 0; i < servers_names.size(); i++)
			if (servers_names[i] == host_header)
				return &(*it);
	}
	return &(servers.front());
}

const LocationConfig* HttpRouter::routeLocation(const ServerConfig& server, const std::string& uri) {
	const std::vector<LocationConfig>& locations = server.getLocations();
	const LocationConfig* best_match = NULL;
	size_t longest_match_length = 0;
	for (std::vector<LocationConfig>::const_iterator it = locations.begin(); it != locations.end(); it++) {
		const std::string& route_path = it->getPath();
		if (uri.find(route_path) == 0)
			if (route_path.length() > longest_match_length) {
				longest_match_length = route_path.length();
				best_match = &(*it);
			}
	}
	return (best_match);
}