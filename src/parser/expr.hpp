#ifndef EXPR_HPP
#define EXPR_HPP

#include <memory>

#include "../scanner/token.hpp"
#include "runtime_error.hpp"

class Expr {
   public:
      virtual ~Expr() = default;
      
      virtual std::string print() const = 0;

      virtual Object interpret() const = 0;

      friend std::ostream& operator<<(std::ostream& out, const Expr& expr) {
	 return out << expr.print();
      }
};

using ExprPtr = std::unique_ptr<Expr>;

class Binary final : public Expr {
   private: 
      ExprPtr m_left;
      Token m_op;
      ExprPtr m_right;

   public:
      Binary(ExprPtr left, Token op, ExprPtr right) 
      : m_left{std::move(left)}, m_op{std::move(op)}, m_right{std::move(right)} 
      {
      }

      std::string print() const override;

      Object interpret() const override; 
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

      Object interpret() const override;

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

      Object interpret() const override;

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

      Object interpret() const override;
};

#endif

   

