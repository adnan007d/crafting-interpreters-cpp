#include "interpreter.h"
#include "lox.h"
#include <iostream>

std::any Interpreter::visitLiteralExpr(const Literal *expr) {
  return expr->value;
}

std::any Interpreter::visitGroupingExpr(const Grouping *expr) {
  return evaluate(expr->expression.get());
}

std::any Interpreter::visitUnaryExpr(const Unary *expr) {

  std::any right = evaluate(expr->right.get());

  switch (expr->op.type) {
  case TokenType::MINUS:
    checkNumberOperand(expr->op, right);
    return -std::any_cast<double>(right);
  case TokenType::BANG:
    return !isTruthy(right);
  }

  return std::any{};
}

std::any Interpreter::visitBinaryExpr(const Binary *expr) {
  std::any left = evaluate(expr->left.get());
  std::any right = evaluate(expr->right.get());

  switch (expr->op.type) {
  case TokenType::MINUS:
    checkNumberOperands(expr->op, left, right);
    return std::any_cast<double>(left) - std::any_cast<double>(right);

  case TokenType::SLASH:
    checkNumberOperands(expr->op, left, right);
    return std::any_cast<double>(left) / std::any_cast<double>(right);

  case TokenType::STAR:
    checkNumberOperands(expr->op, left, right);
    return std::any_cast<double>(left) * std::any_cast<double>(right);

  case TokenType::PLUS:
    if (left.type() == typeid(double) && right.type() == typeid(double)) {
      return std::any_cast<double>(left) + std::any_cast<double>(right);
    }
    if (left.type() == typeid(std::string) &&
        right.type() == typeid(std::string)) {
      return std::any_cast<std::string>(left) +
             std::any_cast<std::string>(right);
    }
    throw RuntimeError{expr->op,
                       "Operands must be two numbers or two strings."};
    break;

  case TokenType::GREATER:
    checkNumberOperands(expr->op, left, right);
    return std::any_cast<double>(left) > std::any_cast<double>(right);

  case TokenType::GREATER_EQUAL:
    checkNumberOperands(expr->op, left, right);
    return std::any_cast<double>(left) >= std::any_cast<double>(right);

  case TokenType::LESS:
    checkNumberOperands(expr->op, left, right);
    return std::any_cast<double>(left) < std::any_cast<double>(right);

  case TokenType::LESS_EQUAL:
    checkNumberOperands(expr->op, left, right);
    return std::any_cast<double>(left) <= std::any_cast<double>(right);

  case TokenType::BANG_EQUAL:
    return !isEqual(left, right);

  case TokenType::EQUAL_EQUAL:
    return isEqual(left, right);
  }

  return std::any{};
}

void Interpreter::interpret(const Expr *expr) {
  try {
    std::any value = evaluate(expr);
    std::cout << stringify(value) << '\n';
  } catch (const RuntimeError &error) {
    Lox::runtimeError(error);
  }
}

std::any Interpreter::evaluate(const Expr *expr) { return expr->accept(*this); }

bool Interpreter::isTruthy(std::any value) {
  if (!value.has_value()) {
    return false;
  }

  if (value.type() == typeid(bool)) {
    return std::any_cast<bool>(value);
  }

  return true;
}

bool Interpreter::isEqual(std::any a, std::any b) {
  if (!a.has_value() && !b.has_value()) {
    return true;
  }

  if (!a.has_value() || !b.has_value()) {
    return false;
  }

  if (a.type() != b.type()) {
    return false;
  }

  if (a.type() == typeid(double)) {
    return std::any_cast<double>(a) == std::any_cast<double>(b);
  }

  if (a.type() == typeid(bool)) {
    return std::any_cast<bool>(a) == std::any_cast<bool>(b);
  }

  if (a.type() == typeid(std::string)) {
    return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
  }

  return false;
}

void Interpreter::checkNumberOperand(const Token &op, const std::any &operand) {
  if (operand.type() == typeid(double)) {
    return;
  }
  throw RuntimeError{op, "Operand must be a number."};
}

void Interpreter::checkNumberOperands(const Token &op, const std::any &left,
                                      const std::any &right) {
  if (left.type() == typeid(double) && right.type() == typeid(double)) {
    return;
  }
  throw RuntimeError{op, "Operands must be numbers."};
}

std::string Interpreter::stringify(const std::any &value) {
  if (!value.has_value()) {
    return "nil";
  }

  if (value.type() == typeid(double)) {
    std::string text = std::to_string(std::any_cast<double>(value));
    if (text.ends_with(".0")) {
      // removing .0 from the end of the string
      text.erase(text.end() - 2, text.end());
    }

    return text;
  }

  if (value.type() == typeid(bool)) {
    return std::any_cast<bool>(value) ? "true" : "false";
  }

  if (value.type() == typeid(std::string)) {
    return std::any_cast<std::string>(value);
  }

  return "nil";
}
