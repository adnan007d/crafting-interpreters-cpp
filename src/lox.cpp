#include "lox.h"
#include "scanner.h"
#include "token.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>

bool Lox::hadError = false;

void Lox::run(const std::string &data) {
  auto scnnaer = Scanner(data);

  const auto &tokens = scnnaer.scanTokens();

  for (const auto &token : tokens) {
    std::cout << token << '\n';
  }
}

void Lox::runFile(const std::string &path) {
  auto file = std::ifstream(path);

  const std::string data = std::string(std::istreambuf_iterator<char>(file),
                                       std::istreambuf_iterator<char>());

  std::cout << data << '\n';

  if (Lox::hadError) {
    std::exit(1);
  }

  run(data);

  Lox::hadError = false;
}

void Lox::error(int line, const std::string &message) {
  report(line, "", message);
}

void Lox::report(int line, const std::string &where,
                 const std::string &message) {
  std::cerr << "[line " << line << "] Error" << where + ": " << message << '\n';
  Lox::hadError = true;
}

void Lox::runPrompt() {
  std::string temp{};
  while (std::getline(std::cin, temp)) {
    run(std::move(temp));
  }
}
