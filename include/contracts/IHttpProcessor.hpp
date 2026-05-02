/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IHttpProcessor.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaolive <joaolive@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 02:46:39 by joaolive          #+#    #+#             */
/*   Updated: 2026/05/02 22:31:57 by joaolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class IHttpProcessor {
	public:
		virtual ~IHttpProcessor() {}

		virtual void feedChunk(const char* data, size_t bytes_read) = 0;
		virtual bool isReady() const = 0;
		virtual const std::vector<char>& buildResponse() = 0;
		virtual void reset() = 0;
		virtual bool shouldCloseConnection() const = 0;
		virtual bool hasError() const = 0;
};