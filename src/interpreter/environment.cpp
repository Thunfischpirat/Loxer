#include "environment.hpp"
#include "../runtime_error.hpp"


void Environment::define(std::string name, Object value) {
   m_values.insert({name, value});
}

void Environment::assign(const Token& name, Object value) {
   if (m_values.contains(name.lexeme())) {
      m_values.insert_or_assign(name.lexeme(), value);
      return;
   }

   throw RuntimeError("Undefined variable '" + name.lexeme() + "'.", name);
}

Object Environment::get(const Token& name) {
   if (m_values.contains(name.lexeme())) {
      return m_values.at(name.lexeme());
   }
   throw RuntimeError("Undefined variable '" + name.lexeme() + "'.", name);
}
