#ifndef INTERPRETER
#define INTERPRETER

#include "environment.hpp"
#include "lox_callable.hpp"
#include "../parser/stmt.hpp"


struct Interpreter {

      Environment globals { Environment() };

      Environment env; 

      Interpreter() {
         std::shared_ptr<LoxCallable> clock { std::make_shared<Time>(Time { }) };
         globals.define("clock", clock); 
         env.enclosing = globals;
      }

      Object operator()(const std::unique_ptr<Expression>& stmt); 

      Object operator()(const std::unique_ptr<Print>& stmt);

      Object operator()(const std::unique_ptr<Var>& stmt);

      Object operator()(const std::unique_ptr<Block>& stmt);

      Object operator()(const std::unique_ptr<If>& stmt);

      Object operator()(const std::unique_ptr<While>& stmt);

      Object operator()(const std::shared_ptr<Function>& stmt);

      Object operator()(const std::unique_ptr<Binary>& expr);

      Object operator()(const std::unique_ptr<Logical>& expr);

      Object operator()(const std::unique_ptr<Unary>& expr);

      Object operator()(const std::unique_ptr<Literal>& expr);

      Object operator()(const std::unique_ptr<Grouping>& expr);

      Object operator()(const std::unique_ptr<Variable>& expr);

      Object operator()(const std::unique_ptr<Assign>& expr);

      Object operator()(const std::unique_ptr<Call>& expr);

      Object operator()(std::monostate);

      void executeBlock(const std::vector<StmtPtr>& statements, Environment& environment);

}; 

#endif
