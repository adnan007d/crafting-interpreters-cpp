#include <iostream>

#include "lox.h"

int main(const int argc, const char *argv[]) {
  if (argc > 1) {
    std::cout << "Usage: cpplox [script]";
    std::exit(1);
  }

  auto lox = Lox();

  if (argc == 2) {
    lox.runFile(argv[1]);
  } else {
    lox.runPrompt();
  }
}
