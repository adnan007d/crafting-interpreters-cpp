#ifndef SCANNER
#define SCANNER

#include "token.h"
#include "token_type.h"
#include <string>
#include <unordered_map>
#include <vector>

class Scanner {
public:
  explicit Scanner(std::string _source) : source{std::move(_source)} {}
  const std::vector<Token> &scanTokens();

private:
  bool        isAtEnd();
  void        scanToken();
  char        advance();
  void        addToken(TokenType type);
  void        addToken(TokenType type, std::any literal);
  bool        match(char expected);
  char        peek();
  void        parseString();
  void        parseNumber();
  static bool isDigit(char c);
  char        peekNext();
  static bool isAlpha(char c);
  void        parseIdentifier();
  static bool isAlphaNumeric(char c);

  std::string        source{};
  std::vector<Token> tokens{};

  int start{0};
  int current{0};
  int line{1};

  std::unordered_map<std ::string_view, TokenType> keywords{
      {"and", TokenType::AND},       {"class", TokenType::CLASS},
      {"else", TokenType::ELSE},     {"false", TokenType::FALSE},
      {"for", TokenType::FOR},       {"fun", TokenType::FUN},
      {"if", TokenType::IF},         {"nil", TokenType::NIL},
      {"or", TokenType::OR},         {"print", TokenType::PRINT},
      {"return", TokenType::RETURN}, {"super", TokenType::SUPER},
      {"this", TokenType::THIS},     {"true", TokenType::TRUE},
      {"var", TokenType::VAR},       {"while", TokenType::WHILE},
  };
};

#endif // !SCANNER
