#include "environment.hpp"
#include "../parser/stmt.hpp"

struct Interpreter {
   Environment env { Environment() }; 
    
   void operator()(std::unique_ptr<Expression> stmt); 

   void operator()(std::unique_ptr<Print> stmt);

   void operator()(std::unique_ptr<Var> stmt);

   void operator()(std::monostate);
}; 
