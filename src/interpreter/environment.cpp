#include "environment.hpp"
#include "../runtime_error.hpp"


void Environment::define(std::string name, Object value) {
   m_values.insert({name, value});
}

Object Environment::get(const Token& name) {
   if (m_values.contains(name.lexeme())) {
      return m_values.at(name.lexeme());
   }
   throw RuntimeError("Undefined variable '" + name.lexeme() + "'.", name);
}
