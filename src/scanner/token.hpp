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

class Token {
public:
   using Literal  = std::variant<bool, float, std::string>;

private:
   TokenType m_type;
   std::string m_lexeme;
   Literal m_literal;
   int m_line;

public:

   Token(TokenType type, std::string lexeme, Literal literal, int line);
   
   friend std::ostream& operator<<(std::ostream& os, const Token& token);
   
   std::string lexeme();
};

std::ostream& operator<<(std::ostream& os, const TokenType& type);
std::ostream& operator<<(std::ostream& os, const Token::Literal& literal);

#endif
