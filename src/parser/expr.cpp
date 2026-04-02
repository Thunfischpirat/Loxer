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

Object Literal::interpret(Environment& env) const {
   (void)env;
   return m_lit;
}

Object Grouping::interpret(Environment& env) const {
   (void)env;
   return m_group->interpret(env);
}

Object Unary::interpret(Environment& env) const {
   (void)env;
   Object right { m_expr->interpret(env) }; 

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

Object Variable::interpret(Environment& env) const {
   return env.get(m_name);
}

Object Assign::interpret(Environment& env) const {
   Object value { m_value->interpret(env) }; 
   env.assign(m_name, value);
   return value;
}

Object Logical::interpret(Environment& env) const {
   Object left { m_left->interpret(env) };

   if (m_op.type() == OR) {
      if (isTruthy(left)) {
         return left;
      }
   }
   else {
      if (!isTruthy(left)) {
         return left;
      }
   }
 
   return m_right->interpret(env);
}

Object Binary::interpret(Environment& env) const {
   (void)env;
   Object left { m_left->interpret(env) };
   Object right { m_right->interpret(env) };
   
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

std::string Assign::print() const {
   return "(assign " + m_name.lexeme() + " " + m_value->print() + ")";
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
