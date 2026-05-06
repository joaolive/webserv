/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRouter.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 21:47:00 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/02 22:00:11 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>
#include <http/model/HttpRequestParser.hpp>
#include <config/model/ServerConfig.hpp>
#include <config/model/LocationConfig.hpp>

class HttpRouter {
	private:
		HttpRouter();
		HttpRouter(const HttpRouter& rhs);
		HttpRouter& operator=(const HttpRouter& rhs);
		~HttpRouter();
	public:
		static const ServerConfig* routeServer(const std::vector<ServerConfig>& servers,
												const HttpRequest& request);
		static const LocationConfig* routeLocation(const ServerConfig& server,
													const std::string& uri);
};