#include "expr.h"

class AstPrinter : public Visitor {

public:
  AstPrinter() = default;
  std::string print(Expr *expr);
  std::any visitBinaryExpr(const Binary *expr) override;
  std::any visitGroupingExpr(const Grouping *expr) override;
  std::any visitLiteralExpr(const Literal *expr) override; 
  std::any visitUnaryExpr(const Unary *expr) override;

private:
  template <typename... Expr>
  std::string parenthesize(const std::string &name, Expr... exprs);
};
