#ifndef TOKEN_H
#define TOKEN_H

#include <variant>
#include <string>
#include <iostream>
#include <map>


enum TokenType {
  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  END_OF_FILE,
};

using Object  = std::variant<std::monostate, bool, float, std::string>;

class Token {
   private:
      TokenType m_type;
      std::string m_lexeme;
      Object m_literal;
      std::size_t m_line;

   public:

      Token(TokenType type, std::string lexeme, Object literal, std::size_t line);
      
      friend std::ostream& operator<<(std::ostream& os, const Token& token);
      
      std::string lexeme() const;
      
      TokenType type() const;

      Object literal() const;

      std::size_t line() const;
};

std::ostream& operator<<(std::ostream& os, const TokenType& type);
std::ostream& operator<<(std::ostream& os, const Object& literal);

#endif
