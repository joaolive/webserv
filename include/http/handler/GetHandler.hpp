/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 13:10:52 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/06 14:09:45 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include "contracts/IMethodHandler.hpp"

class GetHandler : public IMethodHandler {
	private:
		std::string _resolvePath(const HttpRequest& request,
								 const ServerConfig* server,
								 const LocationConfig* location);
		std::string _getMimeType(const std::string& path);
		void _serveStaticFile(const std::string& path,
							  off_t file_size,
							  const ServerConfig* server,
							  bool keep_alive,
							  std::vector<char>& response_buffer);
	public:
		GetHandler();
		virtual ~GetHandler();
		void handle(const HttpRequest& request,
					const ServerConfig* server,
					const LocationConfig* location,
					bool keep_alive,
					std::vector<char>& response_buffer);
}