/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IEventHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 13:47:21 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/03 10:23:36 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/epoll.h>
#include <sys/time.h>

class IEventHandler {
	public:
		virtual ~IEventHandler(void) {};
		virtual void event(epoll_event& event) = 0;
		virtual void closeConnection(void) = 0;
		virtual bool isTimeout(time_t now) const = 0;
		virtual void onTimeout(void) = 0;
};
