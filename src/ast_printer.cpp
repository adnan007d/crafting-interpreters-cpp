#include "ast_printer.h"
#include "expr.h"
#include "token.h"

std::string AstPrinter::print(Expr *expr) {
  return std::any_cast<std::string>(expr->accept(*this));
}

std::any AstPrinter::visitBinaryExpr(const Binary *expr) {
  return parenthesize(expr->op.lexeme, expr->left.get(), expr->right.get());
}

std::any AstPrinter::visitGroupingExpr(const Grouping *expr) {
  return parenthesize("group", expr->expression.get());
}

std::any AstPrinter::visitLiteralExpr(const Literal *expr) {
  return expr->value;
}

std::any AstPrinter::visitUnaryExpr(const Unary *expr) {
  return parenthesize(expr->op.lexeme, expr->right.get());
}

template <typename... Exprs>
std::string AstPrinter::parenthesize(const std::string &name, Exprs... exprs) {
  std::string result = "(" + name;

  for (auto &expr : {exprs...}) {
    result += " ";
    auto t = expr->accept(*this);

    if (t.type() == typeid(double)) {
      result += std::to_string(std::any_cast<double>(t));
    } else if (t.type() == typeid(std::string)) {
      result += std::any_cast<std::string>(t);
    } else {
      result += "nil";
    }
  }

  result += ")";

  return result;
}
