
#include <iostream>
#include <arpa/inet.h>
#include "config/model/ServerConfig.hpp"
#include "infra/engine/ServerEngine.hpp"
#include "contracts/IHttpProcessorFactory.hpp"

#include "mockTest.hpp"

int main(void) {
	ServerConfig* config = new ServerConfig();
	config->setPort(8080);
	config->setHost(htonl(INADDR_LOOPBACK));
	config->setRoot("/tmp");
	config->setAutoindex(false);
	config->setClientMaxBodySize(1024);
	config->addIndex("index.html");

	MockHttpProcessorFactory httpFactory;

	ServerEngine::start(config, httpFactory);

	delete config;

	return (0);
}