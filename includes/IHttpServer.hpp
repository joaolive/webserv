/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IHttpServer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 16:18:53 by mhidani           #+#    #+#             */
/*   Updated: 2026/04/27 16:20:28 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class IHttpServer {
public:
	virtual	~IHttpServer(void) {};
	virtual std::string	process(const std::string& raw) = 0;
};
