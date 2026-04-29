/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IConfig.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 09:43:59 by mhidani           #+#    #+#             */
/*   Updated: 2026/04/28 10:37:16 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class IConfig {
public:
	virtual			~IConfig(void) {};
	virtual int		port(void) const = 0;
	virtual void	port(const int& port) = 0;
	virtual int		addrIPv(void) const = 0;
	virtual void	addrIPv(const int& addrIPv) = 0;
	virtual int		tpn(void) const = 0;
	virtual void	tpn(const int& tpn) = 0;
};
