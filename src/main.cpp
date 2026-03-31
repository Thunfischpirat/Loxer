#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "lox.hpp"

std::string read_file_contents(const std::string& filename);

int main(int argc, char *argv[]) {
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc < 3) {
        std::cerr << "Usage: ./main {tokenize, parse, interpret} <filename>" << std::endl;
        return 1;
    }

    const std::string command = argv[1];
    std::string file_contents = read_file_contents(argv[2]);

    if (command == "tokenize") {
        std::vector<Token> tokens { Lox::tokenize(file_contents) };
	for (const auto& token : tokens) {
	    std::cout << token << std::endl;
        }
    } 
    else if (command == "parse") {
// TODO: Parse statements
//        ExprPtr expression { Lox::parse(file_contents) };
//        std::cout << *expression << '\n';
    }
    else if (command == "interpret") {
        Lox::interpret(file_contents); 
    }
    else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}

std::string read_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error reading file: " << filename << std::endl;
        std::exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}
