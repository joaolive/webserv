/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reactor.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 09:38:31 by mhidani           #+#    #+#             */
/*   Updated: 2026/04/27 18:27:31 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Reactor.hpp"

Reactor::Reactor(IConfig* config, IHttpServer* http) {
	_config = config;
	_http = http;
	_servFd = createSocket();
	_eventMonitorFd = createMonitorHandlers();
}

Reactor::~Reactor(void) {
	close(servFd());
	close(_eventMonitorFd);
}

int Reactor::createSocket(void) const {
	int					fd = -1;
	int					opt = -1;
	struct sockaddr_in	addr;

	if (_config->port() < 0)
		throw std::runtime_error("port value"); // TODO: create exception
	if ((fd = socket(_config->addrIPv(), _config->tpn(), 0)) < 0)
		throw std::runtime_error("socket"); // TODO: create exception

	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	addr.sin_family = _config->addrIPv();
	addr.sin_addr.s_addr = INADDR_ANY; // TODO: check security settings
	addr.sin_port = htons(_config->port());

	if ((bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr))) < 0)
		throw std::runtime_error("bind"); // TODO: create exception
	if (listen(fd, SOMAXCONN) < 0)
		throw std::runtime_error("listen"); // TODO: create exception

	fcntl(fd, F_SETFL, O_NONBLOCK);
	return fd;
}

int Reactor::createMonitorHandlers(void) const {
	int	fd = -1;

	if ((fd = epoll_create1(0)) < 0)
		throw std::runtime_error("event monitor"); // TODO: create exception
	return fd;
}

void Reactor::startEventLoop(void) {
	epoll_event	events[EVENTS_LENGTH];
	int			n = -1;
	int			fd = -1;

	while (true) {
		if ((n = epoll_wait(_eventMonitorFd, events, EVENTS_LENGTH, -1)) < 0)
			throw std::runtime_error("epoll_wait"); // TODO: create exception

		for (int i = 0; i < n; i++) {
			IEventHandler* handler = _handlers[events[i].data.fd];
			handler->handleEvent(events[i]);
		}
	}
}

int Reactor::start(void) {
	AcceptHandler* handler = NULL;

	try {
		handler = new AcceptHandler(this, _http);
		addHandler(servFd(), handler);
		startEventLoop();
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl; // TODO: create log system
		return -1;
	}
	return 0;
}

void Reactor::changeHandlerToWrite(const int& fd) {
	epoll_event	ev;

	if (fd < 0)
		throw std::runtime_error("fd"); // TODO: create exception

	ev.events = EPOLLOUT;
	ev.data.fd = fd;
	if (epoll_ctl(_eventMonitorFd, EPOLL_CTL_MOD, fd, &ev) < 0)
		throw std::runtime_error("epoll_ctl"); // TODO: create exception
}

void Reactor::addHandler(const int& fd, IEventHandler* handler) {
	epoll_event	ev;

	if (fd < 0)
		throw std::runtime_error("fd"); // TODO: create exception
	
	ev.events = EPOLLIN;
	ev.data.fd = fd;
	if (epoll_ctl(_eventMonitorFd, EPOLL_CTL_ADD, fd, &ev) < 0)
		throw std::runtime_error("epoll_ctl"); // TODO: create exception

	_handlers[fd] = handler;
}

void Reactor::removeHandler(const int& fd) {
	if (fd > 0) close(fd);
	delete _handlers[fd];
	_handlers.erase(fd);
}

int Reactor::eventMonitorFd(void) const {
	return _eventMonitorFd;
}

int Reactor::servFd(void) const {
	return _servFd;
}

IConfig* Reactor::config(void) const {
	return _config;
}
