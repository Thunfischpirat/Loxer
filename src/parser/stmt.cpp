#include "stmt.hpp" 

void Visitor::operator()(std::unique_ptr<Expression> stmt) {
   stmt->expression()->interpret();
   return;
}

void Visitor::operator()(std::unique_ptr<Print> stmt) {
   Object value { stmt->expression()->interpret() };
   std::cout << value << '\n';
   return;
}

