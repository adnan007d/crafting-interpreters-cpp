#ifndef LOX
#define LOX

#include "interpreter.h"
#include "token.h"
#include <string>
class Lox {
public:
  void runFile(const std::string &path);
  void runPrompt();

  static void error(int line, const std::string &message);
  static void error(const Token &token, const std::string &message);
  static void runtimeError(const RuntimeError &error);
  static void report(int line, const std::string &where,
                     const std::string &message);

private:
  // NOLINTNEXTLINE
  static bool hadError;
  // NOLINTNEXTLINE
  static bool hadRuntimeError;
  void        run(const std::string &data);

  static Interpreter interpreter;
};

#endif // !LOX
