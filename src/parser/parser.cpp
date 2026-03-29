#include "parser.hpp"

#include "../lox.hpp"

ExprPtr Parser::expression() {
   return assignment();
}

ExprPtr Parser::assignment() {
   ExprPtr expr { equality() };
   
   if (match(EQUAL)) {
      Token equals { previous() };
      ExprPtr value { assignment() };
  
      if (auto var = dynamic_cast<Variable*>(expr.get())) {
         Token name { var->name() }; 
         return std::make_unique<Assign>(name, std::move(value)); 
      }
      
      Lox::error(equals, "Invalid assignment target.");
   }
   return expr;
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
   if (match(IDENTIFIER)) {
      return std::make_unique<Variable>(previous());
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

StmtPtr Parser::declaration() {
   try {
      if (match(VAR)) {
         return varDeclaration();   
      }
      return statement();
   } 
   catch (const ParserError& error) {
      synchronize();
      return std::monostate{};
   }
}

StmtPtr Parser::varDeclaration() {
   Token name { consume(IDENTIFIER, "Expect variable name.") };

   ExprPtr initializer { nullptr };
   
   if (match(EQUAL)) {
      initializer = std::move(expression());
   }
   
   consume(SEMICOLON, "Expect ';' after variable declaration.");
   return std::make_unique<Var>(name, std::move(initializer));
}
   
StmtPtr Parser::statement() {
   if (match(PRINT)) {
      return printStatement();
   }
   if (match(LEFT_BRACE)) {
      return std::make_unique<Block>(std::move(block()));
   }
   return expressionStatement();
}

StmtPtr Parser::printStatement() {
   ExprPtr value { expression() };
   consume(SEMICOLON, "Expect ';' after value.");
   return std::make_unique<Print>(std::move(value));
}

StmtPtr Parser::expressionStatement() {
   ExprPtr expr { expression() };
   consume(SEMICOLON, "Expect ';' after expression.");
   return std::make_unique<Expression>(std::move(expr));
}

std::vector<StmtPtr> Parser::block() {
   std::vector<StmtPtr> statements; 
   
   while (!check(RIGHT_BRACE) && !isAtEnd()) {
      statements.push_back(std::move(declaration()));
   }

   consume(RIGHT_BRACE, "Expect '}' after block.");
   return statements;
}

bool Parser::match(std::vector<TokenType> types) {
   for (const TokenType& type : types) {
      if (match(type)) {
         return true;
      }
   }
   return false;
}

bool Parser::match(TokenType type) {
   if (check(type)) {
      advance();
      return true;
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

void Parser::synchronize() {
   advance();
   while (!isAtEnd()) {
      if (previous().type() == SEMICOLON) {
         return;
      }

      switch (peek().type()) {
         case CLASS:
         case FUN:
         case VAR:
         case FOR:
         case IF:
         case WHILE:
         case PRINT:
         case RETURN:
            return;
         default:
            // TODO: Handle other Token types
            return;
      }
      
      advance();
   }
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

std::vector<StmtPtr> Parser::parse() {
  std::vector<StmtPtr> statements;
  while (!isAtEnd()) {
     StmtPtr stmt { declaration() }; 
     if (!std::holds_alternative<std::monostate>(stmt)) {
        statements.push_back(std::move(stmt));
     }
  } 
  return statements;
}
