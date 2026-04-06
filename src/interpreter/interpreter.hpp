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

      Object operator()(const std::unique_ptr<Expression>& stmt); 

      Object operator()(const std::unique_ptr<Print>& stmt);

      Object operator()(const std::unique_ptr<Var>& stmt);

      Object operator()(const std::unique_ptr<Block>& stmt);

      Object operator()(const std::unique_ptr<If>& stmt);

      Object operator()(const std::unique_ptr<While>& stmt);

      Object operator()(const std::unique_ptr<Binary>& expr);

      Object operator()(const std::unique_ptr<Logical>& expr);

      Object operator()(const std::unique_ptr<Unary>& expr);

      Object operator()(const std::unique_ptr<Literal>& expr);

      Object operator()(const std::unique_ptr<Grouping>& expr);

      Object operator()(const std::unique_ptr<Variable>& expr);

      Object operator()(const std::unique_ptr<Assign>& expr);

      Object operator()(const std::unique_ptr<Call>& expr);

      Object operator()(std::monostate);

}; 
