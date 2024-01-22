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
