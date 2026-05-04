/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMethodHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 19:11:33 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/04 19:15:26 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include "http/model/HttpRequestParser.hpp"
#include "config/model/ServerConfig.hpp"
#include "config/model/LocationConfig.hpp"

class IMethodHandler {
	public:
		virtual ~IMethodHandler() {}

		virtual void handle(const HttpRequest& request,
							const ServerConfig* server,
							const LocationConfig* location,
							bool keep_alive,
							std::vector<char>& response_buffer) = 0;
};