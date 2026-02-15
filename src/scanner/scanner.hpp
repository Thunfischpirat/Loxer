#ifndef SCANNER_H
#define SCANNER_H

#include <vector>
#include <string>

#include "token.hpp"

class Scanner {

private:
    std::string m_source;
    std::vector<Token> m_tokens;
    std::size_t m_line;
    std::size_t m_current;
    std::size_t m_start;
    
    static const std::map<std::string, TokenType> m_keywords; 

public:
    Scanner(const std::string& source) : m_source(source), m_line(1), m_current(0), m_start(0) {}
   
    std::vector<Token> scanTokens();

private:
    char advance();

    bool isAtEnd();

    void addToken (TokenType type);
    
    void addToken(TokenType type, Object); 

    void scanToken();

    bool match(char expected); 
    
    char peek();

    void string();
    
    bool isDigit(const char c);
    
    void number();
    
    char peekNext();

    bool isAlpha(const char c);
    
    bool isAlphaNumeric(const char c);
    
    void identifier();
};

#endif
