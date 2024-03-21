#include "ast_printer.h"
#include <iostream>

#include "lox.h"

int main(const int argc, const char *argv[]) {
  if (argc > 1) {
    std::cout << "Usage: cpplox [script]";
    // NOLINTNEXTLINE
    std::exit(1);
  }

  auto lox = Lox();
  Binary expr = Binary(
      new Unary(Token(TokenType::MINUS, "-", std::any{}, 1), new Literal(123.0)),
      Token(TokenType::STAR, "*", std::any{}, 1),
      new Grouping(new Literal(45.67)));

  AstPrinter astPrinter;

  std::cout << "Here" << '\n';

  std::cout << astPrinter.print(&expr) << '\n';

  if (argc == 2) {
    lox.runFile(argv[1]);
  } else {
    lox.runPrompt();
  }
}
