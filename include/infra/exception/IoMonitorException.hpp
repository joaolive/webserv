/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IoMonitorException.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 08:25:36 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/05 11:18:41 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdexcept>
#include <sstream>

class IoMonitorException {
	public:
		class Create : public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class Wait : public std::exception {
			private:
				int	_fd;
			public:
				Wait(const int& fd);
				virtual const char* what() const throw();
		};
		class AddInterestCtrlEvent : public std::exception {
			private:
				int	_fd;
			public:
				AddInterestCtrlEvent(const int& fd);
				virtual const char* what() const throw();
		};
		class RemoveInterestCtrlEvent : public std::exception {
			private:
				int	_fd;
			public:
				RemoveInterestCtrlEvent(const int& fd);
				virtual const char* what() const throw();
		};
		class ModifyInterestCtrlEvent : public std::exception {
			private:
				int	_fd;
			public:
				ModifyInterestCtrlEvent(const int& fd);
				virtual const char* what() const throw();
		};
};
