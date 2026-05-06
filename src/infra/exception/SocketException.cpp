/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketException.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 15:39:40 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/06 14:44:41 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra/exception/SocketException.hpp"

const char* SocketException::Create::what() const throw() {
	return "error attempting to create the socket.";
}

SocketException::SetOptions::SetOptions(const int& fd) {
	std::stringstream	ss;
	
	ss << "error attempting to set options on the open socket in the file "
	   << "descriptor [" << fd << "]";
	_msg = ss.str();
}

SocketException::SetOptions::~SetOptions(void) throw() {
}

const char* SocketException::SetOptions::what() const throw() {
	return _msg.c_str();
}

SocketException::BindPort::BindPort(const int& fd, const uint32_t& port) {
	std::stringstream	ss;
	
	ss << "error linking socket [" << fd << "] to port[" << port << "]";
	_msg = ss.str();
}

SocketException::BindPort::~BindPort(void) throw() {
}

const char* SocketException::BindPort::what() const throw() {
	return _msg.c_str();
}

SocketException::Listening::Listening(const int& fd) {
	std::stringstream	ss;
	
	ss << "is was not possible to establish a connection to the socket [" 
	   << fd << "]";
	_msg = ss.str();
}

SocketException::Listening::~Listening(void) throw() {
}

const char* SocketException::Listening::what() const throw() {
	return _msg.c_str();
}

SocketException::SetOptionFd::SetOptionFd(const std::string& opt, const int& fd) {
	std::stringstream	ss;

	ss << "it was not possible to set the [" << opt << "] option on the "
	   <<"socket's file descriptor [" << fd << "]";
	_msg = ss.str();
}

SocketException::SetOptionFd::~SetOptionFd(void) throw() {
}

const char* SocketException::SetOptionFd::what() const throw() {
	return _msg.c_str();
}
