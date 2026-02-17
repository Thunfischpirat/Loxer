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

ExprPtr Lox::parse(std::string source) { 
   Parser parser { Lox::tokenize(source) };
   ExprPtr expression { parser.parse() };
  
   if (m_hadError) {
      std::exit(65);
   }

   return expression;
}

std::vector<Token> Lox::tokenize(std::string source) {
   Scanner scanner { source };
   std::vector<Token> tokens = scanner.scanTokens();
    
   if (m_hadError) {
      std::exit(65);
   }

   return tokens;
   
}

void Lox::interpret(std::string source) {
     try {
        ExprPtr expression { parse(source) };
	std::cout << expression->interpret() << '\n';
     }
     catch (const RuntimeError& e) {
        error(e.token(), e.what());	 
        std::exit(70);
     }
}
