/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketException.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 15:39:40 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/04 16:20:45 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra/exceptions/SocketException.hpp"

const char* SocketException::Create::what() const throw() {
	return "error attempting to create the socket.";
}

SocketException::SetOptions::SetOptions(const int& fd) : _fd(fd) {}
const char* SocketException::SetOptions::what() const throw() {
	std::stringstream	ss;

	ss << "error attempting to set options on the open socket in the file "
	   << "descriptor [" << _fd << "]";
	return ss.str().c_str();
}

SocketException::BindPort::BindPort(const int& fd, const uint32_t& port)
	:	_fd(fd), _port(port) {}
const char* SocketException::BindPort::what() const throw() {
	std::stringstream	ss;

	ss << "error linking socket [" << _fd << "] to port[" << _port << "]";
	return ss.str().c_str();
}

SocketException::Listening::Listening(const int& fd) : _fd(fd) {}
const char* SocketException::Listening::what() const throw() {
	std::stringstream	ss;

	ss << "is was not possible to establish a connection to the socket [" << _fd
	   << _fd << "]";
	return ss.str().c_str();
}

SocketException::SetOptionFd::SetOptionFd(const std::string& opt, const int& fd)
	:	_option(opt), _fd(fd) {}
const char* SocketException::SetOptionFd::what() const throw() {
	std::stringstream	ss;

	ss << "it was not possible to set the [" << _option << "] option on the "
	   <<"socket's file descriptor [" << _fd << "]";
	return ss.str().c_str();
}
