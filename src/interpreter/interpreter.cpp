#include "interpreter.hpp" 

void Interpreter::operator()(std::unique_ptr<Expression> stmt) {
   stmt->expression()->interpret(env);
   return;
}

void Interpreter::operator()(std::unique_ptr<Print> stmt) {
   Object value { stmt->expression()->interpret(env) };
   std::cout << value << '\n';
   return;
}

void Interpreter::operator()(std::unique_ptr<Var> stmt) {
   Object value { std::monostate{} };
   ExprPtr expr { stmt->initializer() };
   if (expr) {
      value = expr->interpret(env);
   }
   env.define(stmt->name().lexeme(), value);
   return;
}

void Interpreter::operator()(std::monostate) {
   return;
}

