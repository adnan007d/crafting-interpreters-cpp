#include "lox.h"
#include <iostream>

int main(const int argc, const char *argv[]) {
  if (argc == 1) {
    std::cout << "Usage: cpplox [script]";
    std::exit(1);
  }

  auto lox = Lox();

  if (argc == 1) {
    lox.runFile(argv[0]);
  } else {
    lox.runPrompt();
  }
}
