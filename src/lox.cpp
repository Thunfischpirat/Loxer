#include <cstdlib>

#include "lox.hpp"

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

void Lox::runtimeError(const RuntimeError& error) {
   std::cerr << error.what() << "\n[line " << error.token().line() << "]";
   //m_hadRuntimeError;
}

void Lox::report(std::size_t line, std::string where, std::string_view message) {
   std::cerr << "[line " << line << "] Error" << where << ": " << message << '\n'; 
   m_hadError = true;
}

std::vector<StmtPtr> Lox::parse(std::string source) { 
   Parser parser { Lox::tokenize(source) };
   std::vector<StmtPtr> statements { parser.parse() };
  
   if (m_hadError) {
      std::exit(65);
   }

   return statements;
}

std::vector<Token> Lox::tokenize(std::string source) {
   Scanner scanner { source };
   std::vector<Token> tokens = scanner.scanTokens();
   return tokens;
   
}

void Lox::interpret(std::string source) {
   try {
      std::vector<StmtPtr> statements { parse(source) };
      Interpreter interpreter {};
      for (const StmtPtr& statement: statements) {
         std::visit(interpreter, statement);
      }
   }
   catch (const RuntimeError& e) {
      runtimeError(e);	 
      std::exit(70);
   }
}
