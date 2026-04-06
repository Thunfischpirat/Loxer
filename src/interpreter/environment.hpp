#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <unordered_map>
#include <functional>
#include <optional>
#include <string>

#include "../scanner/token.hpp"


struct Environment {

   std::unordered_map<std::string, Object> m_values {}; 
   std::optional<std::reference_wrapper<Environment>> m_enclosing;

   Environment() = default;

   void define(std::string name, Object value);

   void assign(const Token& name, Object value);

   Object get(const Token& name); 
      
};

#endif
