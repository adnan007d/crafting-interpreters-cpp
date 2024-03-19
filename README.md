# Part 1 Implementations from the [crafting interpreteres book](https://craftinginterpreters.com/) in C++


Compiling tool to generate ast code
```bash
g++-13 tools/GenerateAst.cpp -o build/tool -std=c++23
./build/tool ./src
```

Will generate `expr.h` in `src`
