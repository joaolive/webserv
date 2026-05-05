/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IoMonitorException.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 08:27:34 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/05 11:21:20 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IoMonitorException.hpp"

const char* IoMonitorException::Create::what() const throw() {
	return "error creating the input and output monitor.";
}

IoMonitorException::Wait::Wait(const int& fd) : _fd(fd) {}
const char* IoMonitorException::Wait::what() const throw() {
	std::stringstream	ss;

	ss << "failed to wait on event monitor in [" << _fd << "]";
	return ss.str().c_str();
}

IoMonitorException::AddInterestCtrlEvent::AddInterestCtrlEvent(const int& fd) : _fd(fd) {}
const char* IoMonitorException::AddInterestCtrlEvent::what() const throw() {
	std::stringstream	ss;

	ss << "failed to add controller interest on event monitor in [" << _fd << "]";
	return ss.str().c_str();
}

IoMonitorException::RemoveInterestCtrlEvent::RemoveInterestCtrlEvent(const int& fd) : _fd(fd) {}
const char* IoMonitorException::RemoveInterestCtrlEvent::what() const throw() {
	std::stringstream	ss;

	ss << "failed to remove controller interest on event monitor in [" << _fd << "]";
	return ss.str().c_str();
}

IoMonitorException::ModifyInterestCtrlEvent::ModifyInterestCtrlEvent(const int& fd) : _fd(fd) {}
const char* IoMonitorException::ModifyInterestCtrlEvent::what() const throw() {
	std::stringstream	ss;

	ss << "failed to modify controller interest on event monitor in [" << _fd << "]";
	return ss.str().c_str();
}
