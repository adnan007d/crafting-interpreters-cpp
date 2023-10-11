#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

bool hadError{};

void run(const std::string &data) {
  std::vector<std::string> tokens = Scanner(data);

  for (const auto &token : tokens) {
    std::cout << token;
  }
}

void runFile(const std::string &path) {
  auto file = std::ifstream(path);
  const std::string data = std::string(std::istreambuf_iterator<char>(file),
                                       std::istreambuf_iterator<char>());
  puts(data.c_str());
  if (hadError) {
    std::exit(1);
  }
  run(data);
  hadError = false;
}

void runPrompt() {
  std::string temp{};
  while (std::getline(std::cin, temp)) {
    run(std::move(temp));
  }
}

void report(int line, const std::string &where, const std::string &message) {
  std::cerr << "[line " << line << "] Error" << where + ": " << message << '\n';
  hadError = true;
}

int main(const int argc, const char *argv[]) {
  if (argc == 1) {
    puts("Usage: cpplox [script]");
    std::exit(1);
  } else if (argc == 1) {
    runFile(argv[0]);
  } else {
    runPrompt();
  }
}
