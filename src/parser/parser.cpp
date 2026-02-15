#include "parser.hpp"

#include "../lox.hpp"

ExprPtr Parser::expression() {
   return equality();
}

ExprPtr Parser::equality() {
   ExprPtr expr { comparison() };
   
   std::vector<TokenType> types { BANG_EQUAL, EQUAL_EQUAL }; 
   while (match(types)) {
      Token op { previous() };
      ExprPtr right { comparison() };
      expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
   }
   
   return expr;
}

ExprPtr Parser::comparison() {
   ExprPtr expr { term() };
   
   std::vector<TokenType> types { GREATER, GREATER_EQUAL, LESS, LESS_EQUAL }; 
   while (match(types)) {
      Token op { previous() };
      ExprPtr right { term() };
      expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
   }
   
   return expr;
} 

ExprPtr Parser::term() {
   ExprPtr expr { factor() };
  
   std::vector<TokenType> types { MINUS, PLUS }; 
   while (match(types)) {
      Token op { previous() };
      ExprPtr right { factor() };
      expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
   }
   
   return expr;
} 

ExprPtr Parser::factor() {
   ExprPtr expr { unary() };
   
   std::vector<TokenType> types { SLASH, STAR }; 
   while (match(types)) {
      Token op { previous() };
      ExprPtr right { unary() };
      expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
   }
   
   return expr;
} 

ExprPtr Parser::unary() {
   std::vector<TokenType> types { BANG, MINUS };
   if (match(types)) {
      Token op { previous() };
      ExprPtr right { unary() };
      return std::make_unique<Unary>(op, std::move(right));
   }
   return primary();
}

ExprPtr Parser::primary() {
   std::vector<TokenType> types { NUMBER, STRING, TRUE, FALSE, NIL }; 
   if (match(types)) {
      Object lit { previous().literal() };
      return std::make_unique<Literal>(lit);
   }
   if (check(LEFT_PAREN)) {
      advance();
      ExprPtr expr { expression() };   
      consume(RIGHT_PAREN, "Expect ')' after expression.");
      return std::make_unique<Grouping>(std::move(expr));
   }
   Lox::error(peek(), "Expected expression.");
   throw ParserError { "Expected expression. " };
}

bool Parser::match(std::vector<TokenType> types) {
   for (const TokenType& type : types) {
      if (check(type)) {
         advance();
         return true;
      }
   }
   return false;
}

bool Parser::check(TokenType type) {
   if (isAtEnd()) {
      return false;
   }
   return peek().type() == type;
}

Token Parser::advance() {
   if (!isAtEnd()) {
      m_current++;
   }
   return previous();
}

bool Parser::isAtEnd() {
   return peek().type() == TokenType::END_OF_FILE;
}

Token Parser::peek() {
   return m_tokens[m_current];
}

Token Parser::previous() {
   return m_tokens[m_current - 1];
}

Token Parser::consume(TokenType type, std::string_view message) {
   if (check(type)) {
      return advance();
   }
   Lox::error(peek(), message);
   throw ParserError { message };
}

ExprPtr Parser::parse() {
   try {
      return expression(); 
   }
   catch (const ParserError& exception) {
      std::cerr << "A parser exception occured (" << exception.what() << ")\n";
      return nullptr;
   }
}
