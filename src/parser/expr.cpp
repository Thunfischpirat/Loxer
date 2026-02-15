#include "expr.hpp"

const Expr& Binary::left() const {
   return *m_left;
}

const Expr& Binary::right() const {
   return *m_right;
}

Token Binary::op() const {
   return m_op;
}

const Expr& Unary::expression() const {
   return *m_expr;
}

Token Unary::op() const {
   return m_op;
}

const Expr& Grouping::expression() const {
   return *m_group;
}

Object Literal::literal() const {
   return m_lit;
}

std::string Binary::print() const {
   return "(" + m_op.lexeme() + " " + m_left->print() + " " + m_right->print() + ")"; 
}

std::string Unary::print() const {
   return "(" + m_op.lexeme() + " " + m_expr->print() + ")";  
}

std::string Grouping::print() const {
   return "(group " + m_group->print() + ")";
}

std::string Literal::print() const {
   if (std::holds_alternative<bool>(m_lit)) {
      if (std::get<bool>(m_lit)) {
         return "true";
      }
      return "false";
   }
   else if (std::holds_alternative<float>(m_lit)) {
      return std::to_string(std::get<float>(m_lit));
   }
   else {
      return std::get<std::string>(m_lit);
   }
}
