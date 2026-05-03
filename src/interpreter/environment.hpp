#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <unordered_map>
#include <functional>
#include <optional>
#include <string>

#include "../scanner/token.hpp"

struct Environment {

   std::unordered_map<std::string, Object> values {}; 
   std::optional<std::reference_wrapper<Environment>> enclosing;

   Environment() = default;

   explicit Environment(Environment& env) 
   : enclosing{std::ref(env)}
   {
   }

   Environment(const Environment& other)
   : values{other.values}, enclosing{other.enclosing}
   {
   }

   void define(std::string name, Object value);

   void assign(const Token& name, Object value);

   Object get(const Token& name); 
      
};

#endif
