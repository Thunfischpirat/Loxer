#include "stmt.hpp" 

void Interpreter::operator()(std::unique_ptr<Expression> stmt) {
   stmt->expression()->interpret();
   return;
}

void Interpreter::operator()(std::unique_ptr<Print> stmt) {
   Object value { stmt->expression()->interpret() };
   std::cout << value << '\n';
   return;
}

