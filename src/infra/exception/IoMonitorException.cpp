/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IoMonitorException.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 08:27:34 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/06 14:48:14 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra/exception/IoMonitorException.hpp"

const char* IoMonitorException::Create::what() const throw() {
	return "error creating the input and output monitor.";
}

IoMonitorException::Wait::Wait(const int& fd) {
	std::stringstream	ss;

	ss << "failed to wait on event monitor in [" << fd << "]";
	_msg = ss.str();
}

IoMonitorException::Wait::~Wait(void) throw() {
}

const char* IoMonitorException::Wait::what() const throw() {
	return _msg.c_str();
}

IoMonitorException::AddInterestCtrlEvent::AddInterestCtrlEvent(const int& fd) {
	std::stringstream	ss;

	ss << "failed to add controller interest on event monitor in [" << fd << "]";
	_msg = ss.str();
}

IoMonitorException::AddInterestCtrlEvent::~AddInterestCtrlEvent(void) throw() {
}

const char* IoMonitorException::AddInterestCtrlEvent::what() const throw() {
	return _msg.c_str();
}

IoMonitorException::RemoveInterestCtrlEvent::RemoveInterestCtrlEvent(const int& fd) {
	std::stringstream	ss;

	ss << "failed to remove controller interest on event monitor in [" << fd << "]";
	_msg = ss.str();
}

IoMonitorException::RemoveInterestCtrlEvent::~RemoveInterestCtrlEvent(void) throw() {
}

const char* IoMonitorException::RemoveInterestCtrlEvent::what() const throw() {
	return _msg.c_str();
}

IoMonitorException::ModifyInterestCtrlEvent::ModifyInterestCtrlEvent(const int& fd) {
	std::stringstream	ss;

	ss << "failed to modify controller interest on event monitor in [" << fd << "]";
	_msg = ss.str();
}

IoMonitorException::ModifyInterestCtrlEvent::~ModifyInterestCtrlEvent(void) throw() {
}

const char* IoMonitorException::ModifyInterestCtrlEvent::what() const throw() {
	return _msg.c_str();
}
