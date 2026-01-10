#include <cstdlib>

#include "lox.hpp"
#include "scanner/scanner.hpp"

// Define the static member variable
bool Lox::m_hadError = false;

void Lox::error(int line, std::string message) {
   report(line, "", message);
}

void Lox::report(int line, std::string where, std::string message) {
   std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
   m_hadError = true;
}

void Lox::run(std::string source) {
   Scanner scanner { source };
   std::vector<Token> tokens = scanner.scanTokens();
  
   for (const auto& token : tokens) {
       std::cout << token << std::endl;
   }
   
   if (m_hadError)
      std::exit(65);
}
