#ifndef SCANNER_H
#define SCANNER_H

#include <vector>
#include <string>

#include "token.hpp"

class Scanner {

private:
    std::string m_source;
    std::vector<Token> m_tokens;
    int m_line;
    int m_current;
    int m_start;

public:
    Scanner(const std::string& source) : m_source(source), m_line(1), m_current(0), m_start(0) {}
   
    std::vector<Token> scanTokens();

private:
    const char advance();

    bool isAtEnd();

    void addToken (TokenType type);
    
    void addToken(TokenType type, Literal); 

    void scanToken();
};

#endif
