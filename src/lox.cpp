#include "lox.h"
#include "ast_printer.h"
#include "parser.h"
#include "scanner.h"
#include "token.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>

// NOLINTNEXTLINE
bool Lox::hadError = false;

// NOLINTNEXTLINE
void Lox::run(const std::string &data) {
  auto scanner = Scanner(data);

  const auto &tokens = scanner.scanTokens();

  Parser                parser(tokens);
  std::unique_ptr<Expr> expression = parser.parse();

  if (hadError) {
    hadError = false;
    return;
  }

  std::cout << "Parsed expression: \n";

  std::cout << AstPrinter().print(expression.get()) << '\n';
}

void Lox::runFile(const std::string &path) {
  auto file = std::ifstream(path);

  const std::string data = std::string(std::istreambuf_iterator<char>(file),
                                       std::istreambuf_iterator<char>());

  std::cout << data << '\n';

  if (Lox::hadError) {
    // NOLINTNEXTLINE
    std::exit(1);
  }

  run(data);

  Lox::hadError = false;
}

void Lox::error(int line, const std::string &message) {
  report(line, "", message);
}

void Lox::error(const Token &token, const std::string &message) {
  if (token.type == TokenType::_EOF) {
    report(token.line, " at end", message);
  } else {
    report(token.line, " at '" + token.lexeme + "'", message);
  }
}

void Lox::report(int line, const std::string &where,
                 const std::string &message) {
  std::cerr << "[line " << line << "] Error" << where + ": " << message << '\n';
  Lox::hadError = true;
}

void Lox::runPrompt() {
  std::string temp{};
  while (std::getline(std::cin, temp)) {
    run(temp);
  }
}
