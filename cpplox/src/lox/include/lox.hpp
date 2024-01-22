#pragma once

#include <token.hpp>
#include "../../interpreter/include/interpreter.hpp"

namespace cpplox {
    class Lox {
    public:
        static void runFile(const string& filename);
        static void runPrompt();
        static void error(int line, string message);
        static void error(Token* token, string message);

    private:
        static void run(const string& source);
        static void report(int line, string where, string message);

        static bool hadError;
    };
}  // namespace cpplox
