#include "parser.hpp"

#include "../lox.hpp"

ExprPtr Parser::expression() {
   return assignment();
}

ExprPtr Parser::assignment() {
   ExprPtr expr { disjunction() };
   
   if (match(EQUAL)) {
      Token equals { previous() };
      ExprPtr value { assignment() };
  
      if (std::holds_alternative<std::unique_ptr<Variable>>(expr)) {
         Token name { std::get<std::unique_ptr<Variable>>(expr)->name() }; 
         return std::make_unique<Assign>(name, std::move(value)); 
      }
      
      Lox::error(equals, "Invalid assignment target.");
   }
   return expr;
}

ExprPtr Parser::disjunction() {
   ExprPtr expr { conjunction() };
   
   while (match(OR)) {
      Token op { previous() };
      ExprPtr right { conjunction() };
      expr = std::make_unique<Logical>(std::move(expr), op, std::move(right));
   }

   return expr;
}   

ExprPtr Parser::conjunction() {
   ExprPtr expr { equality() };
   
   while (match(AND)) {
      Token op { previous() };
      ExprPtr right = equality();
      expr = std::make_unique<Logical>(std::move(expr), op, std::move(right));
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
   return call();
}

ExprPtr Parser::call() {
   ExprPtr expr { primary() };  
   
   while (true) {
      if (match(LEFT_PAREN)) {
         expr = finishCall(std::move(expr));
      }
      else {
         break;
      }
   }
 
   return expr;
}

ExprPtr Parser::finishCall(ExprPtr callee) {
   std::vector<ExprPtr> arguments;
   if (!check(RIGHT_PAREN)) {
      do {
         if (arguments.size() >= 255) {
            Lox::error(peek(), "Can't have more than 255 arguments.");
         }
         arguments.emplace_back(std::move(expression()));
      } while (match(COMMA));
   }

   Token paren { consume(RIGHT_PAREN, "Expect ')' after arguments.") };

   return std::make_unique<Call>(std::move(callee), paren, std::move(arguments));
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
      if (match(FUN)) {
	 return function("function");
      }
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

   ExprPtr initializer { std::monostate{} };
   
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
   if (match(IF)) {
      return ifStatement();
   }
   if (match(WHILE)) {
      return whileStatement();
   }
   if (match(FOR)) {
      return forStatement();
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

StmtPtr Parser::ifStatement() {
   consume(LEFT_PAREN, "Expect '(' after 'if'.");
   ExprPtr condition { expression() };
   consume(RIGHT_PAREN, "Expect ')' after if condition.");
   
   StmtPtr thenBranch { statement() };
   StmtPtr elseBranch { std::monostate{} };
   
   if (match(ELSE)) {
      elseBranch = statement();
   }
   
   return std::make_unique<If>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

StmtPtr Parser::whileStatement() {
   consume(LEFT_PAREN, "Expect '(' after 'while'.");
   ExprPtr condition { expression() };
   consume(RIGHT_PAREN, "Expect ')' after condition.");
   StmtPtr body { statement() };
 
   return std::make_unique<While>(std::move(condition), std::move(body));
}

StmtPtr Parser::forStatement() {
   consume(LEFT_PAREN, "Expect '(' after 'for'.");
   
   StmtPtr initializer;
   if (match(SEMICOLON)) {
      initializer = std::monostate{};
   }
   else if (match(VAR)) {
      initializer = varDeclaration();
   }
   else {
      initializer = expressionStatement();
   }

   ExprPtr condition;
   if (!check(SEMICOLON)) {
      condition = expression();
   }
   consume(SEMICOLON, "Expect ';' after loop condition.");

   ExprPtr increment;
   if (!check(RIGHT_PAREN)) {
      increment = expression();
   }
   consume(RIGHT_PAREN, "Expect ')' after for clauses.");
  
   StmtPtr body { statement() };

   if (!std::holds_alternative<std::monostate>(increment)) {
      std::vector<StmtPtr> statements;
      statements.push_back(std::move(body));
      statements.emplace_back(std::make_unique<Expression>(std::move(increment))); 
      body = std::make_unique<Block>(std::move(statements));
   }

   if (std::holds_alternative<std::monostate>(condition)) {
      condition = std::make_unique<Literal>(true);
   }

   body = std::make_unique<While>(std::move(condition), std::move(body));

   if (!std::holds_alternative<std::monostate>(initializer)) {
      std::vector<StmtPtr> statements;
      statements.push_back(std::move(initializer));
      statements.push_back(std::move(body));
      body = std::make_unique<Block>(std::move(statements)); 
   }
   
   return body;
}

std::shared_ptr<Function> Parser::function(const std::string& kind) {
   Token name { consume(IDENTIFIER, "Expect "  + kind + " name.") };
   consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
   std::vector<Token> parameters;
   if (!check(RIGHT_PAREN)) {
      do {
         if (parameters.size() >= 255) {
            Lox::error(peek(), "Can't have more than 255 parameters.");
         }
         
         parameters.emplace_back(consume(IDENTIFIER, "Expect parameter name."));
      } while(match(COMMA));
   }
   consume(RIGHT_PAREN, "Expect ')' after parameters.");
   
   consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
   std::vector<StmtPtr> body { block() };
   return std::make_unique<Function>(name, std::move(parameters), std::move(body));
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
