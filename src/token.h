#ifndef TOKEN
#define TOKEN
#include "token_type.h"
#include <ostream>
#include <string>
#include <any>

class Token {
public:
  Token(TokenType _type, std::string _lexeme, std::any _literal, int _line)
      : type{_type}, lexeme{std::move(_lexeme)}, literal{std::move(_literal)},
        line{_line} {}

  friend std::ostream &operator<<(std::ostream &os, const Token &token) {
    os << "token " << static_cast<int>(token.type) << " lexeme " << token.lexeme
       << " line " << token.line;

    return os;
  }

  TokenType    type;
  std::string  lexeme;
  std::any literal;
  int          line;
};

#endif // !TOKEN
