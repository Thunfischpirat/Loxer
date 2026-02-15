#include <string>

#include "scanner/token.hpp"

class Lox {

public:
   static bool m_hadError;
   
   static void error(std::size_t line, std::string_view message);

   static void error(Token token, std::string_view message);

   static void tokenize(std::string source);

   static void parse(std::string source);

private:
   static void report(std::size_t line, std::string where, std::string_view message);

};
