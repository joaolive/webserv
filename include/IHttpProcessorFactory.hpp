/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IHttpProcessorFactory.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 02:55:00 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/01 03:13:06 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <stdint.h>
#include "IHttpProcessor.hpp"

class IHttpProcessorFactory {
	public:
		virtual ~IHttpProcessorFactory() {}

		virtual IHttpProcessor* createProcessor(const std::string& client_ip, uint16_t server_port) = 0;
};