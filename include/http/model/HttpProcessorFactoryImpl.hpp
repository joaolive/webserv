/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpProcessorFactoryImpl.hpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 15:52:54 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/02 17:54:13 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include "contracts/IHttpProcessorFactory.hpp"
#include "config/model/Config.hpp"

class HttpProcessorFactoryImpl : public IHttpProcessorFactory {
	private:
		std::vector<ServerConfig> _dispatchTable[65536];
		
		HttpProcessorFactoryImpl();
		HttpProcessorFactoryImpl(const HttpProcessorFactoryImpl& rhs);
		HttpProcessorFactoryImpl& operator=(const HttpProcessorFactoryImpl& rhs);
	public:
		HttpProcessorFactoryImpl(const Config& config);
		~HttpProcessorFactoryImpl();

		IHttpProcessor* createProcessor(const std::string& client_ip, uint16_t server_port);
};