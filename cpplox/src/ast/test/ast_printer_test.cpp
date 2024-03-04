#include <gtest/gtest.h>

#include <ast_printer.hpp>
#include <parser.hpp>
#include <scanner.hpp>

using namespace cpplox;

namespace cpplox {
    void Lox::error(int line, string message) {}
    void Lox::error(Token* token, string message) {}
    void Lox::report(int line, string where, string message) {}
}

class AstPrinterTest : public testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
        for (auto _x : stmts)
            delete _x;
        stmts.clear();
    }

    std::vector<Stmt*> stmts;
};

TEST_F(AstPrinterTest, LiteralPrinter) {
    string src = "10;";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    stmts = parser.parse();
    AstPrinter printer;
    printer.print(stmts);
}

TEST_F(AstPrinterTest, BinaryExprPrinter) {
    // string src = "{ (10 + 3) * 5 + (-1) ;}";
    string src = "2+3;";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    stmts = parser.parse();
    AstPrinter printer;
    printer.print(stmts);
}

TEST_F(AstPrinterTest, GroupingExprPrinter) {
    // string src = "{ (10 + 3) * 5 + (-1) ;}";
    string src = "(2+3) * -10;";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    stmts = parser.parse();
    AstPrinter printer;
    printer.print(stmts);
}

TEST_F(AstPrinterTest, PrintStmtTest) {
    string src = "print 2 + 5;";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    stmts = parser.parse();
    AstPrinter printer;
    printer.print(stmts);
}

TEST_F(AstPrinterTest, IfStmtTest) {
    string src = "if (2 == 2) print \"Nitin\";";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    stmts = parser.parse();
    AstPrinter printer;
    printer.print(stmts);
}

TEST_F(AstPrinterTest, IfIfStmtTest) {
    string src = "if (2 == 2) if (true) print \"Hello, World\";";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    stmts = parser.parse();
    AstPrinter printer;
    printer.print(stmts);
}

TEST_F(AstPrinterTest, IfElseStmtTest) {
    string src = "if (2 == 2) print \"Hello\"; else print \"World\";";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    stmts = parser.parse();
    AstPrinter printer;
    printer.print(stmts);
}

TEST_F(AstPrinterTest, IfIfElseStmtTest) {
    // string src = "if (2 == 2) { print \"Hello\"; if (3 >= 2) print \"Hello\"; else print \"World\"; }";
    string src = "{ if (2 == 2) print \"Nitin\"; if (3 >= 2) print \"Lodhi\"; }";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    stmts = parser.parse();
    AstPrinter printer;
    printer.print(stmts);
}

TEST_F(AstPrinterTest, VariableExprTest) {
    string src = "a + 5;";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    stmts = parser.parse();
    AstPrinter printer;
    printer.print(stmts);
}

TEST_F(AstPrinterTest, AssingExprTest) {
    string src = "a = a + 5;";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    stmts = parser.parse();
    AstPrinter printer;
    printer.print(stmts);
}

TEST_F(AstPrinterTest, BlockPrinter) {
    string src = "{ 10; 3;}";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    stmts = parser.parse();
    AstPrinter printer;
    printer.print(stmts);
}

TEST_F(AstPrinterTest, WhileStmtTest) {
    // string src = "if (2 == 2) { print \"Hello\"; if (3 >= 2) print \"Hello\"; else print \"World\"; }";
    string src = "while ( i < 5) { print c; i = i + 1;}";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    stmts = parser.parse();
    AstPrinter printer;
    printer.print(stmts);
}

TEST_F(AstPrinterTest, FunctionStmtTest) {
    string src = "fun sum(a, b) { return a + b; }";
    // string src = "sum(3,8);";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    stmts = parser.parse();
    AstPrinter printer;
    printer.print(stmts);
}

TEST_F(AstPrinterTest, RecursionTest) {
    string src = "{fun sum(a, b) { return a + sum(a, b - 1); } sum(3+4, 4-6);}";
    // string src = "sum(3,8);";
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    stmts = parser.parse();
    AstPrinter printer;
    printer.print(stmts);
}

TEST_F(AstPrinterTest, CallExprTest) {
    string src = "sum(3,8);";
    Scanner scanner(src);
    auto& tokens = scanner.scanTokens();
    Parser parser(tokens);
    stmts = parser.parse();
    AstPrinter printer;
    printer.print(stmts);

    // AstPrinter().print(Parser(scanner.scanTokens()).parse());
}