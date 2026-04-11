#include <string>
#include <vector>

#include "scanner/scanner.hpp"
#include "parser/parser.hpp"
#include "interpreter/interpreter.hpp"

class Lox {

public:
   static bool m_hadError;
   
   static void error(std::size_t line, std::string_view message);

   static void error(Token token, std::string_view message);

   static std::vector<Token> tokenize(std::string source);

   static std::vector<StmtPtr> parse(std::string source);

   static void interpret(std::string source);

private:
   static void report(std::size_t line, std::string where, std::string_view message);

};
