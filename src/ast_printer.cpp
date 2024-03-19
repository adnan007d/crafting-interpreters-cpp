#include "ast_printer.h"
#include "expr.h"
#include "token.h"
#include <variant>

std::string AstPrinter::print(Expr *expr) {
  return std::any_cast<std::string>(expr->accept(*this));
}

std::any AstPrinter::visitBinaryExpr(const Binary *expr) {
  // printf("Inside visitBinaryExpr woow\n");
  // std::printf("%s\n",
  //             parenthesize(expr->op.lexeme, expr->left.get(), expr->right.get())
  //                 .c_str());
  return parenthesize(expr->op.lexeme, expr->left.get(), expr->right.get());
}

std::any AstPrinter::visitGroupingExpr(const Grouping *expr) {
  // printf("Inside visitGroupingExpr woow\n");
  // printf("%s\n", parenthesize("group", expr->expression.get()).c_str());
  return parenthesize("group", expr->expression.get());
}

std::any AstPrinter::visitLiteralExpr(const Literal *expr) {
  // printf("Inside visitLiteralExpr woow\n");
  if (std::holds_alternative<std::monostate>(expr->value)) {
    // printf("nil\n");
    return "nil";
  }

  if (std::holds_alternative<double>(expr->value)) {
    // printf("%f\n", std::get<double>(expr->value));
    return std::get<double>(expr->value);
  }

  // printf("%s\n", std::get<std::string>(expr->value).c_str());
  return std::get<std::string>(expr->value);
}

std::any AstPrinter::visitUnaryExpr(const Unary *expr) {
  // printf("Inside visitUnaryExpr woow\n");
  // printf("%s\n", parenthesize(expr->op.lexeme, expr->right.get()).c_str());
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
