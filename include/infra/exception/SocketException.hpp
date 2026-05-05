/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketException.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 15:38:47 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/05 08:29:59 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>
#include <stdexcept>
#include <sstream>

class SocketException {
	public:
		virtual ~SocketException(void);

		class Create : public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class SetOptions : public std::exception {
			private:
				int			_fd;
			public:
				SetOptions(const int& fd);
				virtual const char* what() const throw();
		};
		class BindPort : public std::exception {
			private:
				int			_fd;
				uint32_t	_port;
			public:
				BindPort(const int& fd, const uint32_t& port);
				virtual const char* what() const throw();
		};
		class Listening : public std::exception {
			private:
				int			_fd;
			public:
				Listening(const int& fd);
				virtual const char* what() const throw();
		};
		class SetOptionFd : public std::exception {
			private:
				std::string	_option;
				int			_fd;
			public:
				SetOptionFd(const std::string& opt, const int& fd);
				virtual const char* what() const throw();
		};
};

