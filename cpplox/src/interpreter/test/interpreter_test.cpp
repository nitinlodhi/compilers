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

TEST_F(InterpreterTest, VarDeclTest) {
    // string src = "{var a = 2; if (a < 3) 3; else 2;}";
    string src = "var a = 10;";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Interpreter interpreter;
    auto r = interpreter.interpret(stmts);
    EXPECT_EQ(any_cast<int>(r), 10);
}

TEST_F(InterpreterTest, ExprTest) {
    string src = "3*8;";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Interpreter interpreter;
    auto r = interpreter.interpret(stmts);
    EXPECT_EQ(any_cast<int>(r), 24);
}

TEST_F(InterpreterTest, ComparisionTest) {
    string src = "2==3;";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Interpreter interpreter;
    auto r = interpreter.interpret(stmts);
    EXPECT_EQ(any_cast<bool>(r), false);
}

TEST_F(InterpreterTest, IfTest) {
    string src =
        "               \
        var a = 1;      \
        if (a < 2)      \
            a = 10;     \
        a;              \
    ";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Interpreter interpreter;
    auto r = interpreter.interpret(stmts);
    EXPECT_EQ(any_cast<int>(r), 10);
}

TEST_F(InterpreterTest, BlockTest) {
    string src = "{ 10; }";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Interpreter interpreter;
    auto r = interpreter.interpret(stmts);
    EXPECT_EQ(any_cast<int>(r), 10);
}

TEST_F(InterpreterTest, IfElseTest) {
    string src =
        "                   \
        var a = 11;         \
        if (a < 2) {        \
            a = 10;         \
        } else {            \
            a = 20;         \
        }                   \
    ";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Interpreter interpreter;
    auto r = interpreter.interpret(stmts);
    EXPECT_EQ(any_cast<int>(r), 20);
}

TEST_F(InterpreterTest, WhileTest) {
    string src =
        "               \
        var a = 1;      \
        var i = 0;      \
        while (i < 3) { \
            i = i+1;    \
            a = a * 2;  \
        }               \
    ";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    Interpreter interpreter;
    auto r = interpreter.interpret(stmts);
    EXPECT_EQ(any_cast<int>(r), 8);
}

TEST_F(InterpreterTest, FunctionTest) {
    string src =
        "                   \
        fun sum(a, b) {     \
            return a + b;   \
        }                   \
    ";

    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    auto r = Interpreter().interpret(stmts);
}

TEST_F(InterpreterTest, FunctionCallTest) {
    string src =
        "                   \
        fun sum(a, b) {     \
            return a + b;   \
        }                   \
        sum(2,5);     \
    ";

    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    auto r = Interpreter().interpret(stmts);
    EXPECT_EQ(any_cast<int>(r), 7);
}