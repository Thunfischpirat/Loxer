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
class If;

using StmtPtr = std::variant<std::unique_ptr<Print>,
                             std::unique_ptr<Expression>,
                             std::unique_ptr<Var>,
                             std::unique_ptr<Block>,
                             std::unique_ptr<If>,
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

class If {
   private:
      ExprPtr m_condition;
      StmtPtr m_then;
      StmtPtr m_else;
   public:
      If(ExprPtr condition, StmtPtr then, StmtPtr other)
      : m_condition{std::move(condition)}, m_then{std::move(then)}, m_else{std::move(other)}
      {
      } 
     
      const ExprPtr& condition() const {
         return m_condition;
      }

      const StmtPtr& thenBranch() const {
         return m_then;
      }

      const StmtPtr& elseBranch() const {
         return m_else;
      }
};



#endif
