#ifndef STMT_HPP
#define STMT_HPP

#include "expr.hpp"


class Expression {
   private:
      ExprPtr m_expression;
   public:
      Expression(ExprPtr expression) 
      : m_expression{ std::move(expression) }
      {
      }

      ExprPtr expression() {
         return std::move(m_expression);
      }
};

class Print {
   private:
      ExprPtr m_expression;
   public:
      Print(ExprPtr expression)
      : m_expression{ std::move(expression) }
      {
      }

      ExprPtr expression() {
         return std::move(m_expression);
      }
};

class Var {
   private:
      Token m_name;
      ExprPtr m_initializer;
   public:
      Var(Token name, ExprPtr initializer) 
      : m_name{name}, m_initializer{ std::move(initializer) }
      {
      }

      ExprPtr initializer() {
         return std::move(m_initializer);
      }
      
      Token name() const {
         return m_name;
      }
};

class Block;

using StmtPtr = std::variant<std::unique_ptr<Print>,
                             std::unique_ptr<Expression>,
                             std::unique_ptr<Var>,
                             std::unique_ptr<Block>,
                             std::monostate>;

class Block {
   private:
      std::vector<StmtPtr> m_statements;
   public:
      Block(std::vector<StmtPtr> statements)
      : m_statements{std::move(statements)}
      {
      }
      
      const std::vector<StmtPtr>& statements() const {
         return m_statements;
      }
};



#endif
