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

struct Visitor {
   void operator()(std::unique_ptr<Expression> stmt); 

   void operator()(std::unique_ptr<Print> stmt);
}; 

using Stmt = std::variant<Print, Expression>;
