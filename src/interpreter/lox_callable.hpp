#ifndef LOXCALLABLE_HPP
#define LOXCALLABLE_HPP

#include <chrono>
#include <vector>

#include "../scanner/token.hpp"
#include "../parser/stmt.hpp"

struct Interpreter;

class LoxCallable {
  public: 
     virtual Object call(Interpreter& interpreter, std::vector<Object>& arguments) const = 0;

     virtual int arity() const = 0;

     ~LoxCallable() = default;
};

class LoxFunction : public LoxCallable {
  private:
       const std::shared_ptr<Function> m_declaration;
  public: 
      explicit LoxFunction(const std::shared_ptr<Function> declaration)
      : m_declaration{declaration}
      {
      }

      Object call(Interpreter& interpreter, std::vector<Object>& arguments) const override;

      int arity() const override;
};

class Time : public LoxCallable {
   public:
      Time() = default;

      Object call(Interpreter& interpreter, std::vector<Object>& arguments) const override; 
      
      int arity() const override { return 0; }
};

#endif
