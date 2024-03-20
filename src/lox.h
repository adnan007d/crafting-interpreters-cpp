#ifndef LOX
#define LOX

#include "token.h"
#include <string>
class Lox {
public:
  void runFile(const std::string &path);
  void runPrompt();

  static void error(int line, const std::string &message);
  static void error(const Token &token, const std::string &message);
  static void report(int line, const std::string &where,
                     const std::string &message);

private:
  // NOLINTNEXTLINE
  static bool hadError;
  void        run(const std::string &data);
};

#endif // !LOX
