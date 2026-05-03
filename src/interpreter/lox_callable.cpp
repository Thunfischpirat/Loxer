#include <chrono>

#include "lox_callable.hpp"
#include "interpreter.hpp"

Object Time::call(Interpreter& interpreter, std::vector<Object>& arguments) const {
   const auto t_now { std::chrono::steady_clock::now().time_since_epoch() };
   const auto n_ticks { std::chrono::duration_cast<std::chrono::milliseconds>(t_now).count() };
   return static_cast<float>(n_ticks); 
}

Object LoxFunction::call(Interpreter& interpreter, std::vector<Object>& arguments) const {
   Environment environment { Environment(interpreter.globals) };
   for (int i { 0 }; i < m_declaration->params().size(); i++) {
      environment.define(m_declaration->params()[i].lexeme(), 
                         arguments[i]);
   }

   interpreter.executeBlock(m_declaration->body(), environment);
   return std::monostate{};
}

int LoxFunction::arity() const {
   return m_declaration->params().size();
}
