#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <unordered_map>
#include <string>
#include <memory>

#include "../scanner/token.hpp"

struct Environment {

   std::unordered_map<std::string, Object> values {}; 
   std::shared_ptr<Environment> enclosing;

   Environment() = default;

   explicit Environment(std::shared_ptr<Environment> env) 
   : enclosing{env}
   {
   }

   Environment(const Environment& other)
   : values{other.values}, enclosing{other.enclosing}
   {
   }

   void define(std::string name, Object value);

   void assign(const Token& name, Object value);

   Object get(const Token& name); 
      
   Environment& operator=(const Environment& other) = default;
};

#endif
