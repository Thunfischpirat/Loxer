#ifndef EXPR_HPP
#define EXPR_HPP

#include <memory>
#include <variant>
#include <optional>
#include <functional>

#include "../scanner/token.hpp"
#include "../runtime_error.hpp"

class Binary;
class Logical;
class Unary;
class Grouping;
class Variable;
class Assign;
class Literal;
class Call;

using ExprPtr = std::variant<std::monostate,
                             std::unique_ptr<Binary>,
                             std::unique_ptr<Logical>,
                             std::unique_ptr<Unary>,
                             std::unique_ptr<Grouping>,
                             std::unique_ptr<Variable>,
                             std::unique_ptr<Assign>,
                             std::unique_ptr<Literal>,
                             std::unique_ptr<Call>>;

class Binary {
   protected: 
      ExprPtr m_left;
      Token m_op;
      ExprPtr m_right;

   public:
      Binary(ExprPtr left, Token op, ExprPtr right) 
      : m_left{std::move(left)}, m_op{op}, m_right{std::move(right)} 
      {
      }
 
      const ExprPtr& left() const { return m_left; }

      Token op() const { return m_op; }
 
      const ExprPtr& right() const { return m_right; }
};


class Logical final: public Binary {
   public:
      Logical(ExprPtr left, Token op, ExprPtr right)
      : Binary { std::move(left), op, std::move(right) }
      {
      }
};


class Unary final {
   private:
      Token m_op;
      ExprPtr m_expr;

   public:
      Unary(Token op, ExprPtr expr)
      : m_op{std::move(op)}, m_expr{std::move(expr)}
      {
      }

      Token op() const { return m_op; }
     
      const ExprPtr& expr() const { return m_expr; }
};


class Grouping final {
   private:
      ExprPtr m_group;

   public:
      Grouping(ExprPtr group)
      : m_group{std::move(group)}
      {
      }

      const ExprPtr& group() const { return m_group; }
};


class Variable final {
   private:
      Token m_name;
   public:
      Variable(Token name) 
      : m_name{name}
      {
      }
      
      Token name() const { return m_name; }
};


class Assign final {
   private:
      Token m_name;
      ExprPtr m_value;
   public:
      Assign(Token name, ExprPtr value)
      : m_name{name}, m_value{std::move(value)}
      {
      }
 
      Token name() const { return m_name; }

      const ExprPtr& value() const { return m_value; }
};


class Literal final {
   private:
      Object m_lit;
 
   public:
      Literal(Object lit)
      : m_lit{std::move(lit)}
      {
      }

      Object lit() const { return m_lit; }
};


class Call final {
   private:
      ExprPtr m_callee;
      Token m_paren;
      std::vector<ExprPtr> m_arguments;
   public:
      Call(ExprPtr callee, Token paren, std::vector<ExprPtr> arguments) 
      : m_callee{std::move(callee)}, m_paren{paren}, m_arguments{std::move(arguments)}
      {
      }
   
      const ExprPtr& callee() const { return m_callee; }

      Token paren() const { return m_paren; }

      const std::vector<ExprPtr>& arguments() const { return m_arguments; }
};

#endif

   

