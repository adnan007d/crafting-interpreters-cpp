#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "expr.h"

class RuntimeError : public std::runtime_error {
public:
  explicit RuntimeError(Token _token, const std::string &what_arg)
      : std::runtime_error(what_arg), token{std::move(_token)} {}

  Token token;
};

class Interpreter : public Visitor {

public:
  std::any visitLiteralExpr(const Literal *expr) override;
  std::any visitGroupingExpr(const Grouping *expr) override;
  std::any visitUnaryExpr(const Unary *expr) override;
  std::any visitBinaryExpr(const Binary *expr) override;

  void interpret(const Expr *expr);

private:
  std::any evaluate(const Expr *expr);
  bool     isTruthy(std::any value);
  bool     isEqual(std::any a, std::any b);

  void checkNumberOperand(const Token &op, const std::any &operand);
  void checkNumberOperands(const Token &op, const std::any &left,
                           const std::any &right);

  std::string stringify(const std::any &value);
};

#endif // !INTERPRETER_H
