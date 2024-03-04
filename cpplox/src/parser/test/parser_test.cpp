#include <gtest/gtest.h>

#include <parser.hpp>
#include <scanner.hpp>

using namespace cpplox;

namespace cpplox {
    void Lox::error(int line, string message) {}
    void Lox::error(Token* token, string message) {}
    void Lox::report(int line, string where, string message) {}
}

class ParserTest : public testing::Test {
protected:
    void SetUp() override {
    }
};

TEST_F(ParserTest, BinaryExpressions) {
    std::string emptyStr;
    Scanner scanner("var a = 2+3;");
    vector<Token> tokens = scanner.scanTokens();

    Parser parser(tokens);
    auto stmts = parser.parse();
    for (auto _x : stmts)
        delete _x;
}
