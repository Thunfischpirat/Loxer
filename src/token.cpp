#include "token.hpp"

std::ostream& operator<<(std::ostream& os, const TokenType& type) {
    static const std::map<TokenType, std::string> tokenTypeNames = {
        // Single-character tokens
        {TokenType::LEFT_PAREN, "LEFT_PAREN"},
        {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
        {TokenType::LEFT_BRACE, "LEFT_BRACE"},
        {TokenType::RIGHT_BRACE, "RIGHT_BRACE"},
        {TokenType::COMMA, "COMMA"},
        {TokenType::DOT, "DOT"},
        {TokenType::MINUS, "MINUS"},
        {TokenType::PLUS, "PLUS"},
        {TokenType::SEMICOLON, "SEMICOLON"},
        {TokenType::SLASH, "SLASH"},
        {TokenType::STAR, "STAR"},
        
        // One or two character tokens
        {TokenType::BANG, "BANG"},
        {TokenType::BANG_EQUAL, "BANG_EQUAL"},
        {TokenType::EQUAL, "EQUAL"},
        {TokenType::EQUAL_EQUAL, "EQUAL_EQUAL"},
        {TokenType::GREATER, "GREATER"},
        {TokenType::GREATER_EQUAL, "GREATER_EQUAL"},
        {TokenType::LESS, "LESS"},
        {TokenType::LESS_EQUAL, "LESS_EQUAL"},
        
        // Literals
        {TokenType::IDENTIFIER, "IDENTIFIER"},
        {TokenType::STRING, "STRING"},
        {TokenType::NUMBER, "NUMBER"},
        
        // Keywords
        {TokenType::AND, "AND"},
        {TokenType::CLASS, "CLASS"},
        {TokenType::ELSE, "ELSE"},
        {TokenType::FALSE, "FALSE"},
        {TokenType::FUN, "FUN"},
        {TokenType::FOR, "FOR"},
        {TokenType::IF, "IF"},
        {TokenType::NIL, "NIL"},
        {TokenType::OR, "OR"},
        {TokenType::PRINT, "PRINT"},
        {TokenType::RETURN, "RETURN"},
        {TokenType::SUPER, "SUPER"},
        {TokenType::THIS, "THIS"},
        {TokenType::TRUE, "TRUE"},
        {TokenType::VAR, "VAR"},
        {TokenType::WHILE, "WHILE"},
        
        {TokenType::END_OF_FILE, "EOF"}
    };
    
    auto it = tokenTypeNames.find(type);
    if (it != tokenTypeNames.end()) {
        os << it->second;
    } else {
        os << "UNKNOWN_TOKEN_TYPE";
    }
    
    return os;
}

std::ostream& operator<<(std::ostream& os, const Literal& literal) {
   if (std::holds_alternative<bool>(literal)) {
	os << std::get<bool>(literal);
   } else if (std::holds_alternative<float>(literal)) {
	os << std::get<float>(literal);
   } else {
	os << std::get<std::string>(literal);
   }
   return os;
}


Token::Token(TokenType type, std::string lexeme, Literal literal, int line)
   : m_type(type), m_lexeme(lexeme), m_literal(literal), m_line(line) {
}


std::ostream& operator<<(std::ostream& os, const Token& token) {
   os << token.m_type << " " << token.m_lexeme << " " << token.m_literal; 
   return os;
}


