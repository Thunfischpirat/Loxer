#include "interpreter.hpp" 

void Interpreter::operator()(const std::unique_ptr<Expression>& stmt) {
   stmt->expression()->interpret(m_env);
}

void Interpreter::operator()(const std::unique_ptr<Print>& stmt) {
   Object value { stmt->expression()->interpret(m_env) };
   std::cout << value << '\n';
}

void Interpreter::operator()(const std::unique_ptr<Var>& stmt) {
   Object value { std::monostate{} };
   ExprPtr expr { stmt->initializer() };
   if (expr) {
      value = expr->interpret(m_env);
   }
   m_env.define(stmt->name().lexeme(), value);
}

void Interpreter::operator()(const std::unique_ptr<Block>& stmt) {
   Interpreter interpreter { m_env };
   for (const StmtPtr& statement : stmt->statements()) {
      std::visit(interpreter, statement); 
   }
}

void Interpreter::operator()(std::monostate) {}

