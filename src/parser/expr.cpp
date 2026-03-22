#include <stdexcept>

#include "expr.hpp"

bool isTruthy(Object object) {
   if (std::holds_alternative<std::monostate>(object)) {
      return false;
   }
   if (std::holds_alternative<bool>(object)) {
      return std::get<bool>(object);
   }
   return true;
}

template <typename T>
bool checkType(Object object, Token token) {
   if (!std::holds_alternative<T>(object)) {
      throw RuntimeError("Operand type incorrect", token);
   }
   return true;
}

Object Literal::interpret_impl(EnvRef env) const {
   (void)env;
   return m_lit;
}

Object Grouping::interpret_impl(EnvRef env) const {
   (void)env;
   return m_group->interpret();
}

Object Unary::interpret_impl(EnvRef env) const {
   (void)env;
   Object right { m_expr->interpret() }; 

   switch (m_op.type()) {
      case MINUS:
         checkType<float>(right, m_op);
         return std::get<float>(right);
      case BANG:
         return !isTruthy(right);
      default:
         return std::monostate{};
   }
   return std::monostate{};
}

Object Variable::interpret_impl(EnvRef env) const {
   return env->get().get(m_name);
}

Object Binary::interpret_impl(EnvRef env) const {
   (void)env;
   Object left { m_left->interpret() };
   Object right { m_right->interpret() };
   
   switch(m_op.type()) {
      case MINUS:
         checkType<float>(left, m_op) && checkType<float>(right, m_op);
         return std::get<float>(left) - std::get<float>(right);
      case PLUS:
         if (std::holds_alternative<float>(left) && std::holds_alternative<float>(right)) {
            return std::get<float>(left) + std::get<float>(right);
         }
         if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
            return std::get<std::string>(left) + std::get<std::string>(right);
         }
         throw RuntimeError("Operand type incorrect", m_op);
         break;
      case SLASH:
         checkType<float>(left, m_op) && checkType<float>(right, m_op);
         return std::get<float>(left) / std::get<float>(right);
      case STAR:
         checkType<float>(left, m_op) && checkType<float>(right, m_op);
         return std::get<float>(left) * std::get<float>(right);
      case GREATER:
         checkType<float>(left, m_op) && checkType<float>(right, m_op);
         return std::get<float>(left) > std::get<float>(right);
      case GREATER_EQUAL:
         checkType<float>(left, m_op) && checkType<float>(right, m_op);
         return std::get<float>(left) >= std::get<float>(right);
      case LESS:
         checkType<float>(left, m_op) && checkType<float>(right, m_op);
         return std::get<float>(left) < std::get<float>(right);
      case LESS_EQUAL:
         checkType<float>(left, m_op) && checkType<float>(right, m_op);
         return std::get<float>(left) <= std::get<float>(right);
      case EQUAL_EQUAL:
         return left == right;
      case BANG_EQUAL:
         return left != right;
      default:
         return std::monostate{};
   }
   return std::monostate{};
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

std::string Variable::print() const {
   return "(variable " + m_name.lexeme() + ")";
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
