#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <memory>
#include <exception>

#include "expr.hpp"
#include "../scanner/token.hpp"
#include "stmt.hpp"

class Parser {

   private:
      std::vector<Token> m_tokens;
      size_t m_current { 0 };
      
      ExprPtr expression();

      ExprPtr assignment();

      ExprPtr disjunction();

      ExprPtr conjunction();

      ExprPtr equality();

      ExprPtr comparison();

      ExprPtr term();

      ExprPtr factor();
   
      ExprPtr unary();

      ExprPtr primary();

      StmtPtr declaration();

      StmtPtr varDeclaration();

      StmtPtr statement(); 
  
      StmtPtr printStatement();
 
      StmtPtr expressionStatement();

      std::vector<StmtPtr> block(); 

      StmtPtr ifStatement();

      bool match(std::vector<TokenType> types);
 
      bool match(TokenType type);
  
      bool check(TokenType type);
  
      Token advance();
 
      void synchronize();
     
      bool isAtEnd();

      Token peek();

      Token previous();
 
      Token consume(TokenType type, std::string_view msg);

   public:
      Parser(std::vector<Token> tokens)
      : m_tokens{std::move(tokens)}
      {
      } 

      std::vector<StmtPtr> parse();

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
