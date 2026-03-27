#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <unordered_map>
#include <string>

#include "../scanner/token.hpp"

class Environment {
   private:
      std::unordered_map<std::string, Object> m_values {}; 

   public: 
      Environment() = default;
    
      void define(std::string name, Object value);

      void assign(const Token& name, Object value);
  
      Object get(const Token& name); 
      
};

#endif
