#ifndef LOX
#define LOX

#include <string>
class Lox {
public:
  void run(const std::string &data);
  void runFile(const std::string &path);
  void error(int line, const std::string &message);
  void runPrompt();

private:
  void report(int line, const std::string &where, const std::string &message);

  bool hadError = false;
};

#endif // !LOX
