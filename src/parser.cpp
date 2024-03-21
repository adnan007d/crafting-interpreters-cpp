#include "parser.h"
#include "lox.h"

#include <memory>

std::unique_ptr<Expr> Parser::expression() { return equality(); };

std::unique_ptr<Expr> Parser::equality() {

  std::unique_ptr<Expr> expr = comparison();

  while (match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
    Token op = previous();

    std::unique_ptr<Expr> right = comparison();
    expr = std::make_unique<Binary>(expr.release(), op, right.release());
  }

  return expr;
}

std::unique_ptr<Expr> Parser::comparison() {
  std::unique_ptr<Expr> expr = term();

  while (match(TokenType::GREATER_EQUAL, TokenType::GREATER,
               TokenType::LESS_EQUAL, TokenType::LESS)) {
    Token                 op = previous();
    std::unique_ptr<Expr> right = term();

    expr = std::make_unique<Binary>(expr.release(), op, right.release());
  }

  return expr;
}

std::unique_ptr<Expr> Parser::term() {
  std::unique_ptr<Expr> expr = factor();

  while (match(TokenType::MINUS, TokenType::PLUS)) {
    Token                 op = previous();
    std::unique_ptr<Expr> right = factor();

    expr = std::make_unique<Binary>(expr.release(), op, right.release());
  }

  return expr;
}

std::unique_ptr<Expr> Parser::factor() {
  std::unique_ptr<Expr> expr = unary();

  while (match(TokenType::SLASH, TokenType::STAR)) {
    Token                 op = previous();
    std::unique_ptr<Expr> right = unary();

    expr = std::make_unique<Binary>(expr.release(), op, right.release());
  }

  return expr;
}

// NOLINTNEXTLINE
std::unique_ptr<Expr> Parser::unary() {
  if (match(TokenType::BANG, TokenType::MINUS)) {
    Token                 op = previous();
    std::unique_ptr<Expr> right = unary();
    return std::make_unique<Unary>(op, right.release());
  }
  return primary();
}

// NOLINTNEXTLINE
std::unique_ptr<Expr> Parser::primary() {
  if (match(TokenType::FALSE)) {
    return std::make_unique<Literal>(false);
  }
  if (match(TokenType::TRUE)) {
    return std::make_unique<Literal>(true);
  }
  if (match(TokenType::NIL)) {
    return std::make_unique<Literal>(std::any{});
  }

  if (match(TokenType::NUMBER, TokenType::STRING)) {
    return std::make_unique<Literal>(previous().literal);
  }

  if (match(TokenType::LEFT_PAREN)) {
    std::unique_ptr<Expr> expr = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
    return std::make_unique<Grouping>(expr.release());
  }

  throw error(peek(), "Expect expression.");
}

Token Parser::consume(TokenType type, const std::string &message) {
  if (check(type)) {
    return advance();
  }
  throw std::runtime_error(error(peek(), message));
}

ParserError Parser::error(const Token &token, const std::string &message) {
  Lox::error(token, message);
  return ParserError(message);
}

void Parser::synchronize() {
  advance();
  while (!isAtEnd()) {
    if (previous().type == TokenType::SEMICOLON) {
      return;
    }
    switch (peek().type) {
    case TokenType::CLASS:
    case TokenType::FUN:
    case TokenType::VAR:
    case TokenType::FOR:
    case TokenType::IF:
    case TokenType::WHILE:
    case TokenType::PRINT:
    case TokenType::RETURN:
      return;
    default:
      break;
    }
    advance();
  }
}

template <typename... T>
  requires(std::conjunction_v<std::is_same<T, TokenType>...>)

bool Parser::match(T... types) {
  // NOLINTNEXTLINE
  for (auto type : {types...}) {
    if (check(type)) {
      advance();
      return true;
    }
  }

  return false;
}

bool Parser::check(TokenType type) {
  if (isAtEnd()) {
    return false;
  }
  return peek().type == type;
}

Token Parser::advance() {
  if (!isAtEnd()) {
    current++;
  }
  return previous();
}

bool Parser::isAtEnd() { return peek().type == TokenType::_EOF; }

Token Parser::peek() { return tokens.at(current); }

Token Parser::previous() { return tokens.at(current - 1); }

std::unique_ptr<Expr> Parser::parse() {
  try {
    return expression();
  } catch (ParserError &error) {
    return nullptr;
  }
}
