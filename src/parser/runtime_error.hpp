#include <stdexcept>
#include <string>
#include <string_view>

#include "../scanner/token.hpp"

class RuntimeError : public std::runtime_error {
   private:
      Token m_token;

   public:
      RuntimeError(const std::string& error, Token token) 
      : std::runtime_error{ error }, m_token{ token }
      {
      } 

      Token token() const {
         return m_token;
      }
};
