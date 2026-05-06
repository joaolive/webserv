
#include <iostream>
#include "config/model/ServerConfig.hpp"
#include "infra/engine/ServerEngine.hpp"
#include "contracts/IHttpProcessorFactory.hpp"

#include "mockTest.hpp"

int main(void) {
	ServerConfig*			config = new ServerConfig();
	IHttpProcessorFactory*	httpFactory;

	ServerEngine::start(config, *httpFactory);

	delete config;
	delete httpFactory;

	return (0);
}