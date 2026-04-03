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

      const ExprPtr& expression() const {
         return m_expression;
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

      const ExprPtr& expression() const {
         return m_expression;
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

      const ExprPtr& initializer() const {
         return m_initializer;
      }
      
      Token name() const {
         return m_name;
      }
};

class Block;
class If;
class While;

using StmtPtr = std::variant<std::unique_ptr<Print>,
                             std::unique_ptr<Expression>,
                             std::unique_ptr<Var>,
                             std::unique_ptr<Block>,
                             std::unique_ptr<If>,
                             std::unique_ptr<While>,
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

class While {
   private:
      ExprPtr m_condition;
      StmtPtr m_body; 
   public:
      While(ExprPtr condition, StmtPtr body)
      : m_condition{std::move(condition)}, m_body{std::move(body)} 
      {
      }

      const ExprPtr& condition() const {
         return m_condition;
      }

      const StmtPtr& body() const {
         return m_body;
      }
};

#endif
