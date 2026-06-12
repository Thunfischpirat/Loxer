#ifndef RETURN_HPP
#define RETURN_HPP

#include <exception>

#include "../scanner/token.hpp"

class ReturnE: public std::exception {
   private:
      Object m_value;

   public:
      explicit ReturnE(Object value) 
      : m_value{std::move(value)}
      {
      } 

      const Object& value() const {
         return m_value;
      }

      const char* what() const noexcept override {
         return "return";
      }
};

#endif
