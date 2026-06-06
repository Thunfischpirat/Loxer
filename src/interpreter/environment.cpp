#include "environment.hpp"
#include "../runtime_error.hpp"


void Environment::define(std::string name, Object value) {
   values.insert({name, value});
}

void Environment::assign(const Token& name, Object value) {
   if (values.contains(name.lexeme())) {
      values.insert_or_assign(name.lexeme(), value);
      return;
   }

   if (enclosing) {
      enclosing->assign(name, value);
      return;
   }

   throw RuntimeError("Undefined variable '" + name.lexeme() + "'.", name);
}

Object Environment::get(const Token& name) {
   if (values.contains(name.lexeme())) {
      return values.at(name.lexeme());
   }

   if (enclosing) {
      return enclosing->get(name);
   }
   throw RuntimeError("Undefined variable '" + name.lexeme() + "'.", name);
}
