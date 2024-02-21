#include <gtest/gtest.h>

#include <interpreter.hpp>
#include <parser.hpp>
#include <scanner.hpp>

using namespace cpplox;

namespace cpplox {
    void Lox::error(int line, string message) {}
    void Lox::error(Token* token, string message) {}
    void Lox::report(int line, string where, string message) {}
}

class InterpreterTest : public testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};


TEST_F(InterpreterTest, FunctionCallTest) {
    string src = "fun sum(a, b) { return a + b; } print sum(2,5); ";
    // string src = "var a = 10;";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Interpreter interpreter;
    auto r = interpreter.interpret(stmts);
    EXPECT_EQ(any_cast<int>(r), 7);
}