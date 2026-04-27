#include <iostream>
#include <vector>
#include "readFile.hpp"
#include "lexer.hpp"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Uso: ./webserv [arquivo_config]" << std::endl;
        return 1;
    }

    try {
        std::string content = ReadFile::readFile(argv[1]);
        Lexer lexer;
        std::vector<Token> tokens = lexer.tokenize(content);

        for (size_t i = 0; i < tokens.size(); ++i) {
            std::cout << "Linha " << tokens[i].line 
                      << " | Token: [" << tokens[i].value << "]" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}