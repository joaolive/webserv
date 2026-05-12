/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerEngine.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhidani <mhidani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 19:33:08 by mhidani           #+#    #+#             */
/*   Updated: 2026/05/11 21:05:05 by mhidani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra/engine/ServerEngine.hpp"

static volatile sig_atomic_t g_running = 1;

static void signalHandler(int) {
	g_running = 0;
}

ServerEngine::ServerEngine(const ServerConfig *conf,
						   IHttpProcessorFactory *httpProcFactory) 
	:	_config(conf), 
		_httpProcFactory(httpProcFactory), 
		_socketFd(createServer()), 
		_ioMonitorFd(createIoMonitor()) {
}

ServerEngine::~ServerEngine(void) {
	close(_socketFd);
	close(_ioMonitorFd);
}

int ServerEngine::createServer(void) {
	int fd, opt = 1, flags;
	struct sockaddr_in addr;

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw SocketException::Create();
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw SocketException::SetOptions(fd);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = _config->getHost();
	addr.sin_port = htons(_config->getPort());
	flags = fcntl(fd, F_GETFL, 0);

	if (bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
		throw SocketException::BindPort(fd, _config->getPort());
	if (listen(fd, SOMAXCONN) < 0) // TODO: check SOMAXCONN to backlog conn
		throw SocketException::Listening(fd);
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK))
		throw SocketException::SetOptionFd("non block", fd);
	return fd;
}

int ServerEngine::createIoMonitor(void) {
	int fd = 0;

	if ((fd = epoll_create1(0)) < 0) {
		close(_socketFd);
		throw IoMonitorException::Create();
	}
	return fd;
}

void ServerEngine::onHandler(const int& idx, epoll_event* events) {
	int				fd = events[idx].data.fd;
	IEventHandler*	handler = _handlers[fd];

	if (handler == NULL)
		return;
	
	handler->event(events[idx]);
	switch (handler->stage()) {
		case IEventHandler::ERROR:	std::cerr << "error!";
		case IEventHandler::CLOSED:	removeHandler(fd);	break;
		default:										break;
	}
}

void ServerEngine::dispatchHandler(const int& timeout) {
	epoll_event	events[IO_MONITOR_SIZE];
	const int	nEvents = epoll_wait(_ioMonitorFd, events, IO_MONITOR_SIZE, timeout);

	if (nEvents < 0) throw IoMonitorException::Wait(_ioMonitorFd);

	for (int i = 0; i < nEvents; i++) {
		try {
			onHandler(i, events);
		} catch (const std::exception& e) {
			// TODO: check call exception to http processor and send response error 
			continue;
		}
	}
}

void ServerEngine::startEventLoop(void) {
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
	const int timeout = 5000;

	while (g_running) {
		try {
			dispatchHandler(timeout);

			// Timeout
			_registerTime = time(NULL);
			// std::vector<int> toRemove;
			// std::map<int, IEventHandler *>::iterator it = _handlers.begin();
			// for (; it != _handlers.end(); ++it)
			// 	if (it->second->isTimeout(now))
			// 		toRemove.push_back(it->first);
			// for (size_t i = 0; i < toRemove.size(); ++i)
			// 	removeHandler(toRemove[i]);

		} catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			continue ;
		}
	}
}

int ServerEngine::getSocketFd(void) const {
	return _socketFd;
}

int ServerEngine::getIoMonitorFd(void) const {
	return _ioMonitorFd;
}

const ServerConfig *ServerEngine::getConfig(void) const {
	return _config;
}

IHttpProcessorFactory *ServerEngine::getHttpProcFactory(void) const {
	return _httpProcFactory;
}

IEventHandler *ServerEngine::getHandler(const int &fd) {
	return _handlers[fd];
}

void ServerEngine::addHandler(const int &fd, const uint32_t &events, 
							  IEventHandler *hdl) {
	struct epoll_event ev;

	ev.events = events;
	ev.data.fd = fd;

	if (epoll_ctl(_ioMonitorFd, EPOLL_CTL_ADD, fd, &ev) == -1)
		throw IoMonitorException::AddInterestCtrlEvent(_ioMonitorFd);

	if (static_cast<size_t>(fd) >= _handlers.size())
		_handlers.resize(fd + 1, NULL);

	_handlers[fd] = hdl;
}

void ServerEngine::removeHandler(const int &fd) {
	if (epoll_ctl(_ioMonitorFd, EPOLL_CTL_DEL, fd, NULL) == -1)
		throw IoMonitorException::RemoveInterestCtrlEvent(_ioMonitorFd);

	delete _handlers[fd];
	close(fd);
	_handlers[fd] = NULL;
}

void ServerEngine::changeHandlerState(const int &fd, const uint32_t &state) {
	struct epoll_event ev;

	ev.events = state;
	ev.data.fd = fd;

	if (epoll_ctl(_ioMonitorFd, EPOLL_CTL_MOD, fd, &ev) < 0)
		throw IoMonitorException::ModifyInterestCtrlEvent(_ioMonitorFd);
}

time_t ServerEngine::getRegisterTime(void) const {
	return _registerTime;
}
