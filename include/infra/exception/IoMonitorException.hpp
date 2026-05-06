/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IoMonitorException.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 08:25:36 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/06 14:46:03 by mhidani          ###   ########.fr       */
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
				std::string	_msg;
			public:
				Wait(const int& fd);
				virtual ~Wait(void) throw();
				virtual const char* what() const throw();
		};
		class AddInterestCtrlEvent : public std::exception {
			private:
				std::string	_msg;
			public:
				AddInterestCtrlEvent(const int& fd);
				virtual ~AddInterestCtrlEvent(void) throw();
				virtual const char* what() const throw();
		};
		class RemoveInterestCtrlEvent : public std::exception {
			private:
				std::string	_msg;
			public:
				RemoveInterestCtrlEvent(const int& fd);
				virtual ~RemoveInterestCtrlEvent(void) throw();
				virtual const char* what() const throw();
		};
		class ModifyInterestCtrlEvent : public std::exception {
			private:
				std::string	_msg;
			public:
				ModifyInterestCtrlEvent(const int& fd);
				virtual ~ModifyInterestCtrlEvent(void) throw();
				virtual const char* what() const throw();
		};
};
