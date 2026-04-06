#include "interpreter.hpp" 

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

Object Interpreter::operator()(const std::unique_ptr<Expression>& stmt) {
   std::visit(*this, stmt->expression());
   return std::monostate{};
}

Object Interpreter::operator()(const std::unique_ptr<Print>& stmt) {
   Object value { std::visit(*this, stmt->expression()) };
   std::cout << value << '\n';
   return std::monostate{};
}

Object Interpreter::operator()(const std::unique_ptr<Var>& stmt) {
   Object value { std::monostate{} };
   const ExprPtr& expr { stmt->initializer() };
   if (!std::holds_alternative<std::monostate>(expr)) {
      value = std::visit(*this, expr);
   }
   m_env.define(stmt->name().lexeme(), value);
   return std::monostate{};
}

Object Interpreter::operator()(const std::unique_ptr<Block>& stmt) {
   Interpreter interpreter { m_env };
   for (const StmtPtr& statement : stmt->statements()) {
      std::visit(*this, statement); 
   }
   return std::monostate{};
}

Object Interpreter::operator()(const std::unique_ptr<If>& stmt) { 
   Interpreter interpreter { m_env };
   if (isTruthy(std::visit(*this, stmt->condition()))) {
      std::visit(*this, stmt->thenBranch());
   }
   else {
      std::visit(*this, stmt->elseBranch());
   }
   return std::monostate{};
}

Object Interpreter::operator()(const std::unique_ptr<While>& stmt) {
   Interpreter interpreter { m_env };
   while (isTruthy(std::visit(*this, stmt->condition()))) {
      std::visit(*this, stmt->body());
   }
   return std::monostate{};
}

Object Interpreter::operator()(const std::unique_ptr<Literal>& expr) {
   return expr->lit();
}

Object Interpreter::operator()(const std::unique_ptr<Grouping>& expr) {
   return std::visit(*this, expr->group());
}

Object Interpreter::operator()(const std::unique_ptr<Unary>& expr) {
   Object right { std::visit(*this, expr->expr()) }; 

   Token m_op { expr->op() };

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

Object Interpreter::operator()(const std::unique_ptr<Variable>& expr) {
   return m_env.get(expr->name());
}

Object Interpreter::operator()(const std::unique_ptr<Assign>& expr) {
   Object value { std::visit(*this, expr->value()) }; 
   m_env.assign(expr->name(), value);
   return value;
}

Object Interpreter::operator()(const std::unique_ptr<Logical>& expr) {
   Object left { std::visit(*this, expr->left()) };

   if (expr->op().type() == OR) {
      if (isTruthy(left)) {
         return left;
      }
   }
   else {
      if (!isTruthy(left)) {
         return left;
      }
   }
 
   return std::visit(*this, expr->right());
}

Object Interpreter::operator()(const std::unique_ptr<Call>& expr) {
   Object callee { std::visit(*this, expr->callee()) };
   std::vector<Object> arguments;

   for (const ExprPtr& arg : expr->arguments()) {
      arguments.push_back(std::visit(*this, arg));
   }
/*
   if (!std::holds_alternative<LoxCallable>(callee)) {
      throw RuntimeError("Can only call functions and classes.", m_paren);
   }

   if (arguments.size() != function.arity()) {
      throw RuntimeError("Expected " 
                         + function.arity() 
                         + " arguments but got " 
                         + arguments.size()
                         +  ".");
   }
   
   // TODO:Implement LoxCallable
   LoxCallable function { static_cast<LoxCallable>(callee) };
*/
   return callee; // function.call(env, arguments);
}

Object Interpreter::operator()(const std::unique_ptr<Binary>& expr) {
   Object left { std::visit(*this, expr->left()) };
   Object right { std::visit(*this, expr->right()) };
   
   Token m_op { expr->op() };

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

Object Interpreter::operator()(std::monostate) { return std::monostate{}; }

