#ifndef EXPR_HPP
#define EXPR_HPP

#include <memory>
#include <optional>
#include <functional>

#include "../scanner/token.hpp"
#include "../interpreter/environment.hpp"
#include "../runtime_error.hpp"

bool isTruthy(Object);

class Expr {
   public:

      virtual ~Expr() = default;
      
      virtual std::string print() const = 0;

      virtual Object interpret(Environment& env) const = 0;

      friend std::ostream& operator<<(std::ostream& out, const Expr& expr) {
	 return out << expr.print();
      }
};

using ExprPtr = std::unique_ptr<Expr>;

class Binary : public Expr {
   protected: 
      ExprPtr m_left;
      Token m_op;
      ExprPtr m_right;

   public:
      Binary(ExprPtr left, Token op, ExprPtr right) 
      : m_left{std::move(left)}, m_op{op}, m_right{std::move(right)} 
      {
      }

      std::string print() const override;

      Object interpret(Environment& env) const override; 
};

class Logical final: public Binary {

   public:
      Logical(ExprPtr left, Token op, ExprPtr right)
      : Binary { std::move(left), op, std::move(right) }
      {
      }
	 
      Object interpret(Environment& env) const override;

};

class Unary final : public Expr {
   private:
      Token m_op;
      ExprPtr m_expr;

   public:
      Unary(Token op, ExprPtr expr)
      : m_op{std::move(op)}, m_expr{std::move(expr)}
      {
      }
     
      std::string print() const override;

      Object interpret(Environment& env) const override;

};

class Grouping final : public Expr {
   private:
      ExprPtr m_group;

   public:
      Grouping(ExprPtr group)
      : m_group{std::move(group)}
      {
      }

      std::string print() const override;

      Object interpret(Environment& env) const override;

};

class Variable final : public Expr {
   private:
      Token m_name;
   public:
      Variable(Token name) 
      : m_name{name}
      {
      }
      
      Token name() const {
         return m_name;
      }

      std::string print() const override;
    
      Object interpret(Environment& env) const override;
};

class Assign final: public Expr {
   private:
      Token m_name;
      ExprPtr m_value;
   public:
      Assign(Token name, ExprPtr value)
      : m_name{name}, m_value{std::move(value)}
      {
      }
 
      Token name() const {
         return m_name;
      }
      
      std::string print() const override;

      Object interpret(Environment& env) const override; 
};

class Literal final : public Expr {
   private:
      Object m_lit;
 
   public:
      Literal(Object lit)
      : m_lit{std::move(lit)}
      {
      }
   
      std::string print() const override;

      Object interpret(Environment& env) const override;
};

#endif

   

