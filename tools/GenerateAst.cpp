#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

std::string tolower_stdstr(const std::string &s) {
  std::string result = s;
  std::transform(result.begin(), result.end(), result.begin(), ::tolower);
  return result;
}

bool isWhiteSpace(const char c) {
  return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

std::string trim(std::string s) {
  size_t start = 0;
  while (start < s.size() && isWhiteSpace(s[start])) {
    ++start;
  }
  s.erase(0, start);

  size_t end = s.size() - 1;
  while (end <= 0 && isWhiteSpace(s[end])) {
    --end;
  }
  s.erase(end + 1);

  return s;
}
std::vector<std::string> split(const std::string &s, const std::string &delim) {

  std::vector<std::string> v{};
  v.reserve(s.size() / 2);

  auto start = s.begin();

  while (start <= s.end()) {
    auto found = std::search(start, s.end(), delim.begin(), delim.end());
    v.emplace_back(start, found);
    start = found + delim.size();
  }

  return v;
}

void defineVisitor(std::ofstream &writer, const std::string &baseName,
                   const std::vector<std::string> &types) {
  writer << "class Visitor{\n";
  writer << "public:\n";
  for (const auto &type : types) {
    auto typeName = trim(split(type, ":")[0]);
    writer << "\tvirtual std::any visit" + typeName + "(const " + typeName + " &" +
                  baseName + ") = 0;\n";
  }

  writer << "\tvirtual ~Visitor() = default;\n";
  writer << "};\n\n";
}

void defineType(std::ofstream &writer, const std::string &baseName,
                const std::string &className, const std::string &fields) {
  writer << "class " + className + " : public " + baseName + "{\n";

  auto splittedFields = split(fields, std::string(", "));

  writer << "public:\n";
  writer << "\t" + className + "(" + fields + "): ";
  for (const auto &field : splittedFields) {
    auto fieldName = split(field, " ")[1];
    writer << "_" + fieldName + "{std::move(" + fieldName + ")}\n";
    if (field != *(splittedFields.end() - 1)) {
      writer << ", ";
    } else {
      writer << " {}\n";
    }
  }

  writer << "std::any accept(Visitor &visitor) override {\n";
  writer << "\tvisitor.visit" + className + "(*this);\n";
  writer << "}\n";

  for (const auto &field : splittedFields) {
    auto fieldName = split(field, " ");
    writer << "\t" + fieldName[0] + " _" + fieldName[1] + ";\n";
  }
  writer << "\n};\n\n";
}

void defineAst(const std::string &outputDir, const std::string &baseName,
               const std::vector<std::string> &types) {

  std::string   path = outputDir + "/" + tolower_stdstr(baseName) + ".h";
  std::ofstream outfile = std::ofstream(path);

  outfile << "#ifndef " + baseName + "_H\n";
  outfile << "#define " + baseName + "_H\n";
  outfile << "#include <any>\n";
  outfile << "#include \"token.h\"\n";
  outfile << "class Visitor;\n\n";
  outfile << "class " + baseName + "{";
  outfile << "public:\n";
  outfile << "\tvirtual std::any accept(Visitor &visitor) {}\n";
  outfile << "};\n\n";

  for (const auto &type : types) {
    auto        splits = split(type, ":");
    std::string className = trim(splits[0]);
    std::string fields = trim(splits[1]);
    outfile << "class " + className << ";\n";
  }

  defineVisitor(outfile, baseName, types);

  for (const auto &type : types) {
    auto        splits = split(type, ":");
    std::string className = trim(splits[0]);
    std::string fields = trim(splits[1]);

    defineType(outfile, baseName, className, fields);
  }

  outfile << "#endif\n";
}

int main(const int argc, const char *argv[]) {
  if (argc == 1) {
    printf("Usage: generate_ast <output directory>\n");
    return 1;
  }

  std::string outputDir = argv[1];
  std::printf("Generating AST to %s\n", outputDir.c_str());

  defineAst(outputDir, "Expr",
            std::vector<std::string>(
                {"Binary   : Expr left, Token op, Expr right",
                 "Grouping : Expr expression", "Literal  : literal_type value",
                 "Unary    : Token op, Expr right"}));
}
