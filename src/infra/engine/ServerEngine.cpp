/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerEngine.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 19:33:08 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/06 15:02:46 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra/engine/ServerEngine.hpp"

ServerEngine::ServerEngine(ServerConfig* config)
	:	_socketFd(createServer()),
		_ioMonitorFd(createIoMonitor()), 
		_config(config) {
}

ServerEngine::~ServerEngine(void) {
	close(_socketFd);
	close(_ioMonitorFd);
}

int ServerEngine::createServer(void) {
	int					fd, opt, flags;
	struct sockaddr_in	addr;

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw SocketException::Create();
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw SocketException::SetOptions(fd);

	addr.sin_family = AF_INET;
	addr.sin_family = htonl(_config->getHost());
	addr.sin_port = htons(_config->getPort());
	flags = fcntl(fd, F_GETFL, 0);

	if (bind(_socketFd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
		throw SocketException::BindPort(fd, _config->getPort());
	if (listen(fd, SOMAXCONN) < 0) // TODO: check SOMAXCONN to backlog conn
		throw SocketException::Listening(fd);
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) )
		throw SocketException::SetOptionFd("non block", fd);
	return fd;
}

int ServerEngine::createIoMonitor(void) {
	int	fd = 0;

	if ((fd = epoll_create1(0)) < 0) {
		close(_socketFd);
		throw IoMonitorException::Create();
	}
	return fd;
}

void ServerEngine::startEventLoop(void) {
	epoll_event		events[IO_MONITOR_SIZE];
	const int		timeout = 5000;
	int				nEvents = 0, fd = 0;
	time_t			now;

	while (true) {
		nEvents = epoll_wait(_ioMonitorFd, events, IO_MONITOR_SIZE, timeout);
		if (nEvents < 0)
			throw IoMonitorException::Wait(_ioMonitorFd);

		for (int i = 0; i < nEvents; i++) {
			fd = events[i].data.fd;
			_handlers[fd]->event(events[i]);
		}

		now = time(NULL);
		std::vector<int> toRemove;
		std::map<int, IEventHandler*>::iterator it = _handlers.begin();
		for (; it != _handlers.end(); ++it)
			if (it->second->isTimeout(now))
				toRemove.push_back(it->first);
		for (size_t i = 0; i < toRemove.size(); ++i)
			removeHandler(toRemove[i]);
	}
}

int ServerEngine::start(ServerConfig* config, IHttpProcessorFactory& factory) {
	try {
		ServerEngine	server(config);
		int				fd = server.getSocketFd();
		AcceptHandler*	acceptHandler = new AcceptHandler(fd, &server, factory); 
		
		server.addHandler(fd, EPOLLIN, acceptHandler);
		server.startEventLoop();

	} catch (const std::exception& e) {
		std::cerr << "error: " << e.what() << std::endl; // TODO: create log capture
		return -1;
	}
	return 0;
}

int ServerEngine::getSocketFd(void) const {
	return _socketFd;
}

int ServerEngine::getIoMonitorFd(void) const {
	return _ioMonitorFd;
}

ServerConfig* ServerEngine::getConfig(void) const {
	return _config;
}

IEventHandler* ServerEngine::getHandler(const int& fd) {
	return _handlers[fd];
}

void ServerEngine::addHandler(const int& fd, const uint32_t& events, IEventHandler* hdl) {
	struct epoll_event	ev;

	ev.events = events;
	ev.data.fd = fd;

	if (epoll_ctl(_ioMonitorFd, EPOLL_CTL_ADD, fd, &ev) == -1)
		throw IoMonitorException::AddInterestCtrlEvent(_ioMonitorFd);
	_handlers[fd] = hdl;
}

void ServerEngine::removeHandler(const int& fd) {
	if (epoll_ctl(_ioMonitorFd, EPOLL_CTL_DEL, fd, NULL) == -1)
		throw IoMonitorException::RemoveInterestCtrlEvent(_ioMonitorFd);

	delete _handlers[fd];
	_handlers.erase(fd);
	close(fd);
}

void ServerEngine::changeHandlerState(const int& fd, const uint32_t& state) {
	struct epoll_event	ev;

	ev.events = state;
	ev.data.fd = fd;
	
	if (epoll_ctl(_ioMonitorFd, EPOLL_CTL_MOD, fd, &ev) < 0)
		throw IoMonitorException::ModifyInterestCtrlEvent(_ioMonitorFd);
}
