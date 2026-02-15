#include <cstdlib>

#include "lox.hpp"
#include "scanner/scanner.hpp"
#include "parser/parser.hpp"

// Define the static member variable
bool Lox::m_hadError = false;

void Lox::error(std::size_t line, std::string_view message) {
   report(line, "", message);
}

void Lox::error(Token token, std::string_view message) {
   if (token.type() == TokenType::END_OF_FILE) {
      report(token.line(), " at end", message);
   }
   else {
      report(token.line(), " at '" + token.lexeme() + "'", message);
   }
}

void Lox::report(std::size_t line, std::string where, std::string_view message) {
   std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
   m_hadError = true;
}

void Lox::parse(std::string source) {
   Scanner scanner { source };
   std::vector<Token> tokens = scanner.scanTokens();
  
   Parser parser { tokens };
   ExprPtr expression { parser.parse() };
  
   if (m_hadError) {
      std::exit(65);
   }

   std::cout << *expression << '\n';
}

void Lox::tokenize(std::string source) {
   Scanner scanner { source };
   std::vector<Token> tokens = scanner.scanTokens();
  
   for (const auto& token : tokens) {
       std::cout << token << std::endl;
   }
   
   if (m_hadError) {
      std::exit(65);
   }
}
