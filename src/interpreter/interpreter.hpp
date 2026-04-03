#include "environment.hpp"
#include "../parser/stmt.hpp"

class Interpreter {
   private:
      Environment m_env { Environment() }; 

   public:

      Interpreter() = default;

      Interpreter(Environment& env)
      : m_env{env}
      {
      }

      void operator()(const std::unique_ptr<Expression>& stmt); 

      void operator()(const std::unique_ptr<Print>& stmt);

      void operator()(const std::unique_ptr<Var>& stmt);

      void operator()(const std::unique_ptr<Block>& stmt);

      void operator()(const std::unique_ptr<If>& stmt);

      void operator()(const std::unique_ptr<While>& stmt);

      void operator()(std::monostate);

}; 
