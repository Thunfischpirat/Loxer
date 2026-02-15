#include "token.hpp"

std::ostream& operator<<(std::ostream& os, const TokenType& type) {
    static const std::map<TokenType, std::string> tokenTypeNames = {
        // Single-character tokens
        {LEFT_PAREN, "LEFT_PAREN"},
        {RIGHT_PAREN, "RIGHT_PAREN"},
        {LEFT_BRACE, "LEFT_BRACE"},
        {RIGHT_BRACE, "RIGHT_BRACE"},
        {COMMA, "COMMA"},
        {DOT, "DOT"},
        {MINUS, "MINUS"},
        {PLUS, "PLUS"},
        {SEMICOLON, "SEMICOLON"},
        {SLASH, "SLASH"},
        {STAR, "STAR"},
        
        // One or two character tokens
        {BANG, "BANG"},
        {BANG_EQUAL, "BANG_EQUAL"},
        {EQUAL, "EQUAL"},
        {EQUAL_EQUAL, "EQUAL_EQUAL"},
        {GREATER, "GREATER"},
        {GREATER_EQUAL, "GREATER_EQUAL"},
        {LESS, "LESS"},
        {LESS_EQUAL, "LESS_EQUAL"},
        
        // Literals
        {IDENTIFIER, "IDENTIFIER"},
        {STRING, "STRING"},
        {NUMBER, "NUMBER"},
        
        // Keywords
        {AND, "AND"},
        {CLASS, "CLASS"},
        {ELSE, "ELSE"},
        {FALSE, "FALSE"},
        {FUN, "FUN"},
        {FOR, "FOR"},
        {IF, "IF"},
        {NIL, "NIL"},
        {OR, "OR"},
        {PRINT, "PRINT"},
        {RETURN, "RETURN"},
        {SUPER, "SUPER"},
        {THIS, "THIS"},
        {TRUE, "TRUE"},
        {VAR, "VAR"},
        {WHILE, "WHILE"},
        
        {END_OF_FILE, "EOF"}
    };
    
    auto it = tokenTypeNames.find(type);
    if (it != tokenTypeNames.end()) {
        os << it->second;
    } else {
        os << "UNKNOWN_TOKEN_TYPE";
    }
    
    return os;
}

std::ostream& operator<<(std::ostream& os, const Object& literal) {
   if (std::holds_alternative<bool>(literal)) {
	os << std::get<bool>(literal);
   } else if (std::holds_alternative<float>(literal)) {
	os << std::get<float>(literal);
   } else {
	os << std::get<std::string>(literal);
   }
   return os;
}


Token::Token(TokenType type, std::string lexeme, Object literal, std::size_t line)
   : m_type(type), m_lexeme(lexeme), m_literal(literal), m_line(line) {
}

std::string Token::lexeme() const {
   return m_lexeme;
}

TokenType Token::type() const {
   return m_type;
}

Object Token::literal() const {
   return m_literal;
}

std::size_t Token::line() const {
   return m_line;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
   os << token.m_type << " " << token.m_lexeme << " " << token.m_literal; 
   return os;
}

   


