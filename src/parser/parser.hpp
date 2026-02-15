#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <memory>
#include <exception>

#include "expr.hpp"
#include "../scanner/token.hpp"

class Parser {

   private:
      std::vector<Token> m_tokens;
      size_t m_current { 0 };
      
      ExprPtr expression();

      ExprPtr equality();

      ExprPtr comparison();

      ExprPtr term();

      ExprPtr factor();
   
      ExprPtr unary();

      ExprPtr primary();

      bool match(std::vector<TokenType> types);
  
      bool check(TokenType type);
  
      Token advance();
     
      bool isAtEnd();

      Token peek();

      Token previous();
 
      Token consume(TokenType type, std::string_view msg);

   public:
      Parser(std::vector<Token> tokens)
      : m_tokens{std::move(tokens)}
      {
      } 

      ExprPtr parse();

};

class ParserError : public std::exception {
   private:
      std::string m_error;
   public:
      ParserError(std::string_view error)
      : m_error{error}
      {
      }
      
      const char* what() const noexcept override { return m_error.c_str(); };
};

#endif
