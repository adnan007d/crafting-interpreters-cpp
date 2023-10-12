#ifndef TOKEN
#define TOKEN
#include "token_type.h"
#include <ostream>
#include <string>

class Token {
public:
  Token(TokenType _type, std::string _lexeme, std::string _literal, int _line)
      : type{_type}, lexeme{std::move(_lexeme)}, literal{std::move(_literal)},
        line{_line} {}

  friend std::ostream &operator<<(std::ostream &os, const Token &token) {
    os << static_cast<int>(token.type) << ' ' << token.lexeme << ' '
       << token.line;

    return os;
  }

private:
  TokenType type;
  std::string lexeme;
  std::string literal;
  int line;
};

#endif // !TOKEN
