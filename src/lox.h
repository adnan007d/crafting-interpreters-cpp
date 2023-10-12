#ifndef LOX
#define LOX

#include <string>
class Lox {
public:
  void run(const std::string &data);
  void runFile(const std::string &path);
  void runPrompt();

  static void error(int line, const std::string &message);
  static void report(int line, const std::string &where,
                     const std::string &message);

  static bool hadError;
};

#endif // !LOX
