#include <string>

class Lox {

public:
   static bool m_hadError;
   
   static void error(int line, std::string message);

   static void run(std::string source);

private:
   static void report(int line, std::string where, std::string message);

};
