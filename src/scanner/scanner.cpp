#include "scanner.hpp"
#include "../lox.hpp"

using namespace std::string_literals;

const std::map<std::string, TokenType> Scanner::m_keywords = {
    {"and",    AND},
    {"class",  CLASS},
    {"else",   ELSE},
    {"false",  FALSE},
    {"for",    FOR},
    {"fun",    FUN},
    {"if",     IF},
    {"nil",    NIL},
    {"or",     OR},
    {"print",  PRINT},
    {"return", RETURN},
    {"super",  SUPER},
    {"this",   THIS},
    {"true",   TRUE},  
    {"var",    VAR},
    {"while",  WHILE}   
};    

std::vector<Token> Scanner::scanTokens() {
   while (!isAtEnd()) {
      m_start = m_current;
      scanToken();
   }

   m_tokens.push_back(Token { TokenType::END_OF_FILE, "", "null", m_line }); 
   return m_tokens;   
}

char Scanner::advance() {
   return m_source.at(m_current++);
}

bool Scanner::isAtEnd() {
   return m_current >= m_source.length();
}

void Scanner::addToken(TokenType type) {
   addToken(type, "null");
}

void Scanner::addToken(TokenType type, Object literal) {
   std::string text = m_source.substr(m_start, m_current - m_start);
   m_tokens.push_back(Token { type, text, literal, m_line });
}

void Scanner::scanToken() {
   const char c = advance();
   switch(c) {
      case '(' : addToken(LEFT_PAREN); break;
      case ')' : addToken(RIGHT_PAREN); break;
      case '}' : addToken(LEFT_PAREN); break;
      case '{' : addToken(RIGHT_PAREN); break;
      case ',' : addToken(COMMA); break;
      case '.' : addToken(DOT); break;
      case '-' : addToken(MINUS); break;
      case '+' : addToken(PLUS); break;
      case ';' : addToken(SEMICOLON); break;
      case '*' : addToken(STAR); break;
      case '!':
        addToken(match('=') ? BANG_EQUAL : BANG);
        break;
      case '=':
        addToken(match('=') ? EQUAL_EQUAL : EQUAL);
        break;
      case '<':
        addToken(match('=') ? LESS_EQUAL : LESS);
        break;
      case '>':
        addToken(match('=') ? GREATER_EQUAL : GREATER);
        break;
      case '/':
	if (match('/')) {
	   while (peek() != '\n' && !isAtEnd())
	      advance();
	}
	else 
	   addToken(SLASH);
	break;
      case '"': string(); break;
      case ' ' :
      case '\r':
      case '\t':
	break;
      case '\n': 
	m_line++;
	break;
      default:
	 if (isDigit(c)) 
	    number();
	 else if (isAlpha(c))
 	    identifier();
	 else
            Lox::error(m_line, "Unexpected character: "s + c);  
	 break;
   }
}

bool Scanner::match(char expected) {
   if (isAtEnd())
	return false;   
   if (m_source.at(m_current) != expected)
	return false;
   m_current++;
   return true;
}

char Scanner::peek() {
   if (isAtEnd()) 
	return '\0';
   return m_source.at(m_current);
}

void Scanner::string() {
   while (peek() != '"' && !isAtEnd()) {
      if (peek() == '\n') 
         m_line++;
      advance();
   }      

   if (isAtEnd()) {
      Lox::error(m_line, "Unterminated string");
   }

   advance();

   std::string value { m_source.substr(m_start + 1, m_current - m_start - 2) };
   addToken(STRING, value);
}

bool Scanner::isDigit(const char c) {
   return c >= '0' && c <= '9';
}

void Scanner::number() {
   while (isDigit(peek()))
      advance();
   if (peek() == '.' && isDigit(peekNext())) {
      advance();
      while (isDigit(peek()))
         advance();
   }
   float value { std::stof(m_source.substr(m_start, m_current - m_start)) };
   addToken(NUMBER, value);
}

char Scanner::peekNext() {
   if (m_current + 1 >= m_source.length())
      return '\0';
   return m_source.at(m_current + 1);
}

bool Scanner::isAlpha(const char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

bool Scanner::isAlphaNumeric(const char c) {
   return isDigit(c) || isAlpha(c);
}

void Scanner::identifier() {
   while (isAlphaNumeric(peek()))
      advance();

   std::string text { m_source.substr(m_start, m_current - m_start) };
   auto it { m_keywords.find(text) };
   if (it != m_keywords.end()) 
      addToken(it->second);
   else
      addToken(IDENTIFIER);
}
