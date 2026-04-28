
#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>

#include "IConfig.hpp"
#include "IHttpServer.hpp"
#include "IServ.hpp"
#include "Reactor.hpp"

class Config : public IConfig {
private:
	int	_port;
	int	_addrIPv;
	int	_tpn;

public:
			Config(void) { }
	virtual	~Config(void) { }
	int		port(void) const { return _port; }
	void	port(const int& port) { _port = port; }
	int		addrIPv(void) const { return _addrIPv; }
	void	addrIPv(const int& addrIPv) { _addrIPv = addrIPv; }
	int		tpn(void) const { return _tpn; }
	void	tpn(const int& tpn) { _tpn = tpn; }
};

class HttpWebServer : public IHttpServer {
public:
				HttpWebServer(void) {}
	virtual		~HttpWebServer(void) {}
	std::string	process(const std::string& raw) {
		std::string response = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 46\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<html><body><h1>Webserv 42 Test</h1></body></html>";
		return response;
	}
};


int main(void) {
	IConfig*		config = NULL;
	IHttpServer*	http = NULL;
	Reactor*		serv = NULL;

	config = new Config();
	config->port(8080);
	config->addrIPv(AF_INET);
	config->tpn(SOCK_STREAM);

	http = new HttpWebServer();

	serv = new Reactor(config, http);
	serv->start();

	delete serv;
	delete http;
	delete config;

	return (0);
}
