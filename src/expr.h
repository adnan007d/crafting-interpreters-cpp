#ifndef Expr_H
#define Expr_H
#include "token.h"
#include <any>
#include <memory>
class Visitor;

class Expr {
public:
  // Default move constructor and move assignment operator are deleted
  Expr() = default;
  Expr(const Expr &) = default;
  Expr(Expr &&) = delete;

  Expr &operator=(const Expr &) = default;
  Expr &operator=(Expr &&) = delete;

  virtual ~Expr() = default;
  virtual std::any accept(Visitor &visitor) const = 0;
};

class Binary;
class Grouping;
class Literal;
class Unary;
class Visitor {
public:
  Visitor() = default;
  Visitor(const Visitor &) = default;
  Visitor(Visitor &&) = delete;
  Visitor         &operator=(const Visitor &) = default;
  Visitor         &operator=(Visitor &&) = delete;
  virtual std::any visitBinaryExpr(const Binary *Expr) = 0;
  virtual std::any visitGroupingExpr(const Grouping *Expr) = 0;
  virtual std::any visitLiteralExpr(const Literal *Expr) = 0;
  virtual std::any visitUnaryExpr(const Unary *Expr) = 0;
  virtual ~Visitor() = default;
};

class Binary : public Expr {
public:
  explicit Binary(Expr *_left, Token _op, Expr *_right)
      : left{_left}, op{std::move(_op)}, right{_right} {}

  std::any accept(Visitor &visitor) const override {
    return visitor.visitBinaryExpr(this);
  }

  std::unique_ptr<Expr> left;
  Token                 op;
  std::unique_ptr<Expr> right;
};

class Grouping : public Expr {
public:
  explicit Grouping(Expr *_expression) : expression{_expression} {}

  std::any accept(Visitor &visitor) const override {
    return visitor.visitGroupingExpr(this);
  }

  std::unique_ptr<Expr> expression;
};

class Literal : public Expr {
public:
  explicit Literal(std::any _value) : value{std::move(_value)} {}

  std::any accept(Visitor &visitor) const override {
    return visitor.visitLiteralExpr(this);
  }

  std::any value;
};

class Unary : public Expr {
public:
  Unary(Token _op, Expr *_right) : op{std::move(_op)}, right{_right} {}

  std::any accept(Visitor &visitor) const override {
    return visitor.visitUnaryExpr(this);
  }

  Token                 op;
  std::unique_ptr<Expr> right;
};

#endif // !Expr_H
