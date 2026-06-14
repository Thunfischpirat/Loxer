#ifndef LOXCALLABLE_HPP
#define LOXCALLABLE_HPP

#include <chrono>
#include <vector>

#include "../scanner/token.hpp"
#include "../parser/stmt.hpp"
#include "environment.hpp"

struct Interpreter;

class LoxCallable {
  public: 
     virtual Object call(Interpreter& interpreter, std::vector<Object>& arguments) const = 0;

     virtual std::size_t arity() const = 0;

     virtual std::string toString() const = 0;

     ~LoxCallable() = default;
};

class LoxFunction : public LoxCallable {
  private:
       const std::shared_ptr<Function> m_declaration;
       const std::shared_ptr<Environment> m_closure;
  public: 
      explicit LoxFunction(const std::shared_ptr<Function> declaration, std::shared_ptr<Environment> closure)
      : m_declaration{declaration} , m_closure{closure}
      {
      }

      Object call(Interpreter& interpreter, std::vector<Object>& arguments) const override;

      std::size_t arity() const override;

      std::string toString() const override;
};

class Time : public LoxCallable {
   public:
      Time() = default;

      Object call(Interpreter& interpreter, std::vector<Object>& arguments) const override; 
      
      std::size_t arity() const override { return 0; }

      std::string toString() const override;
};

#endif
