#include <iostream>
#include <vector>
#include "readFile.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "config.hpp"

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Uso: ./webserv [arquivo_config]" << std::endl;
		return 1;
	}

	try {
		// 1. Ler e Tokenizar
		std::string content = ReadFile::readFile(argv[1]);
		Lexer lexer;
		std::vector<Token> tokens = lexer.tokenize(content);

		// 2. Fazer o Parsing
		Config config;
		if (Parser::parse(config, tokens) != 0) {
			std::cerr << "Erro fatal: Falha ao fazer o parsing do arquivo de configuracao." << std::endl;
			return 1;
		}

		std::cout << "\n[+] Parsing concluido com sucesso! Montando AST...\n" << std::endl;

		// =====================================================================
		// DEBUG: IMPRIMINDO A ÁRVORE DE CONFIGURAÇÃO (AST)
		// =====================================================================
		const std::vector<ServerConfig>& servers = config.getServers();
		
		for (size_t i = 0; i < servers.size(); ++i) {
			std::cout << "========================================\n";
			std::cout << " SERVER [" << i + 1 << "]\n";
			std::cout << "========================================\n";
			std::cout << "  Porta: " << servers[i].getPort() << "\n";
			std::cout << "  Root: " << (servers[i].getRoot().empty() ? "(vazio)" : servers[i].getRoot()) << "\n";
			std::cout << "  Max Body Size: " << servers[i].getClientMaxBodySize() << "\n";
			
			// Testando o mapa de Error Pages do Server (Modo C++98 usando const_iterator)
			const std::map<uint16_t, std::string>& server_errors = servers[i].getErrorPages();
			for (std::map<uint16_t, std::string>::const_iterator it = server_errors.begin(); it != server_errors.end(); ++it) {
				std::cout << "  Error Page [" << it->first << "]: " << it->second << "\n";
			}

			const std::vector<LocationConfig>& locs = servers[i].getLocations();
			for (size_t j = 0; j < locs.size(); ++j) {
				std::cout << "\n  >>> Location [" << locs[j].getPath() << "]\n";
				// Este root aqui DEVE ser igual ao do Server se o usuário não tiver digitado nada no location!
				std::cout << "      Root Herdado/Definido: " << locs[j].getRoot() << "\n"; 
				std::cout << "      Autoindex: " << (locs[j].getAutoindex() ? "ON" : "OFF") << "\n";
				
				// Testando o mapa de Error Pages do Location para ver se a fusão (merge) funcionou
				const std::map<uint16_t, std::string>& loc_errors = locs[j].getErrorPages();
				for (std::map<uint16_t, std::string>::const_iterator it = loc_errors.begin(); it != loc_errors.end(); ++it) {
					std::cout << "      Error Page [" << it->first << "]: " << it->second << "\n";
				}
			}
			std::cout << "\n";
		}

	} catch (const std::exception& e) {
		std::cerr << "Excecao capturada: " << e.what() << std::endl;
		return 1;
	}
	
	return 0;
}