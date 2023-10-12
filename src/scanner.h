#ifndef SCANNER
#define SCANNER

#include "token.h"
#include "token_type.h"
#include <string>
#include <vector>

class Scanner {
public:
  Scanner(std::string _source) : source{std::move(_source)} {}
  const std::vector<Token> &scanTokens();

private:
  std::string source{};
  std::vector<Token> tokens{};

  int start{0};
  int current{0};
  int line{1};

  bool isAtEnd();
  void scanToken();
  char advance();
  void addToken(TokenType token);
  void addToken(TokenType type, std::string literal);
  bool match(char expected);
};

#endif // !SCANNER
