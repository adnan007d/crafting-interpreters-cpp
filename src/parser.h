#ifndef PARSER
#define PARSER

#include "expr.h"
#include "token.h"
#include "token_type.h"
#include <vector>

class ParserError : public std::runtime_error {
public:
  explicit ParserError(const std::string &message)
      : std::runtime_error{message} {}
};

class Parser {

public:
  explicit Parser(std::vector<Token> _tokens) : tokens{std::move(_tokens)} {}

  std::unique_ptr<Expr> parse();
private:
  std::unique_ptr<Expr> expression();
  std::unique_ptr<Expr> equality();
  std::unique_ptr<Expr> comparison();
  std::unique_ptr<Expr> term();
  std::unique_ptr<Expr> factor();
  std::unique_ptr<Expr> unary();
  std::unique_ptr<Expr> primary();

  template <typename... T>
    requires(std::conjunction_v<std::is_same<T, TokenType>...>)
  bool match(T... types);

  bool        check(TokenType type);
  Token       advance();
  bool        isAtEnd();
  Token       peek();
  Token       previous();
  Token       consume(TokenType type, const std::string &message);
  ParserError error(const Token &token, const std::string &message);
  void synchronize();


  std::vector<Token> tokens;
  int                current = 0;
};

#endif // !PARSER
