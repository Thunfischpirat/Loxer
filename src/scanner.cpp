#include "scanner.hpp"

std::vector<Token> Scanner::scanTokens() {
   while (!isAtEnd()) {
      m_start = m_current;
      scanToken();
   }

   m_tokens.push_back(Token { TokenType::END_OF_FILE, "", "null", m_line }); 
   return m_tokens;   
}

const char Scanner::advance() {
   return m_source.at(m_current++);
}

bool Scanner::isAtEnd() {
   return m_current >= m_source.length();
}

void Scanner::addToken(TokenType type) {
   addToken(type, "null");
}

void Scanner::addToken(TokenType type, Literal literal) {
   std::string text = m_source.substr(m_start, m_current - m_start);
   m_tokens.push_back(Token { type, text, literal, m_line });
}

void Scanner::scanToken() {
   const char c = advance();
   switch(c) {
      case '(' : addToken(TokenType::LEFT_PAREN); break;
      case ')' : addToken(TokenType::RIGHT_PAREN); break;
   }
}
