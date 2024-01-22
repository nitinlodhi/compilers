#include <gtest/gtest.h>

#include <scanner.hpp>
using namespace cpplox;

namespace cpplox {
    void Lox::error(int line, string message) {}
    void Lox::error(Token* token, string message) {}
    void Lox::report(int line, string where, string message) {}
}

class ScannerTest : public testing::Test {
protected:
    void SetUp() override {
    }
};

TEST_F(ScannerTest, NullString) {
    std::string emptyStr;
    Scanner scanner(emptyStr);
    const vector<Token>& tokens = scanner.scanTokens();
    
    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, EOF_);
}

TEST_F(ScannerTest, EmptyString) {
    std::string emptyStr("");
    Scanner scanner(emptyStr);
    const vector<Token>& tokens = scanner.scanTokens();
    
    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, EOF_);
}

TEST_F(ScannerTest, StringLiteral) {
    Scanner scanner("\"Lox\"");
    const vector<Token>& tokens = scanner.scanTokens();
    
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, STRING);
    EXPECT_EQ(any_cast<string>(tokens[0].literal), "Lox");

    EXPECT_EQ(tokens[1].type, EOF_);
}

TEST_F(ScannerTest, IntegerLiteral) {
    Scanner scanner("120820");
    const vector<Token>& tokens = scanner.scanTokens();
    
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, INTEGER);
    EXPECT_EQ(any_cast<int>(tokens[0].literal), 120820);

    EXPECT_EQ(tokens[1].type, EOF_);
}

TEST_F(ScannerTest, DoubleLiteral) {
    Scanner scanner("3.14");
    const vector<Token>& tokens = scanner.scanTokens();
    
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, DOUBLE);
    EXPECT_EQ(any_cast<double>(tokens[0].literal), 3.14);

    EXPECT_EQ(tokens[1].type, EOF_);
}

TEST_F(ScannerTest, Identifier) {
    Scanner scanner("x");
    const vector<Token>& tokens = scanner.scanTokens();
    
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, IDENTIFIER);

    EXPECT_EQ(tokens[1].type, EOF_);
}

TEST_F(ScannerTest, VarDecl) {
    Scanner scanner("var x = 10;");
    const vector<Token>& tokens = scanner.scanTokens();
    
    EXPECT_EQ(tokens.size(), 6);
    EXPECT_EQ(tokens[0].type, VAR);
    EXPECT_EQ(tokens[1].type, IDENTIFIER);
    EXPECT_EQ(tokens[2].type, EQUAL);
    EXPECT_EQ(tokens[3].type, INTEGER);
    EXPECT_EQ(any_cast<int>(tokens[3].literal), 10);
    EXPECT_EQ(tokens[4].type, SEMICOLON);
    EXPECT_EQ(tokens[5].type, EOF_);
}

TEST_F(ScannerTest, IfStmt) {
    string src = "\
        if (true) {\n\
            print a;\n\
        }\n";

    Scanner scanner(src);
    const vector<Token>& tokens = scanner.scanTokens();
    
    EXPECT_EQ(tokens.size(), 10);
    EXPECT_EQ(tokens[0].type, IF);
    EXPECT_EQ(tokens[1].type, LEFT_PAREN);
    EXPECT_EQ(tokens[2].type, TRUE);
    EXPECT_EQ(tokens[3].type, RIGHT_PAREN);
    EXPECT_EQ(tokens[4].type, LEFT_BRACE);
    EXPECT_EQ(tokens[4].line, 1);
    EXPECT_EQ(tokens[5].type, PRINT);
    EXPECT_EQ(tokens[5].line, 2);
    EXPECT_EQ(tokens[6].type, IDENTIFIER);
    EXPECT_EQ(tokens[6].lexeme, "a");
    EXPECT_EQ(tokens[7].type, SEMICOLON);
    EXPECT_EQ(tokens[7].line, 2);
    EXPECT_EQ(tokens[8].type, RIGHT_BRACE);
    EXPECT_EQ(tokens[8].line, 3);
    EXPECT_EQ(tokens[9].type, EOF_);
}

TEST_F(ScannerTest, IfElseStmt) {
    string src = "\
        if (true) {\n\
            print a;\n\
        } else {\n\
            if (false)\n\
                print b;\n\
            else {\n\
                print c;\n\
            }\n\
        ";

    Scanner scanner(src);
    const vector<Token>& tokens = scanner.scanTokens();
    
    EXPECT_EQ(tokens.size(), 25);
    int idx = 0;
    EXPECT_EQ(tokens[idx++].type, IF);
    EXPECT_EQ(tokens[idx++].type, LEFT_PAREN);
    EXPECT_EQ(tokens[idx++].type, TRUE);
    EXPECT_EQ(tokens[idx++].type, RIGHT_PAREN);
    EXPECT_EQ(tokens[idx++].type, LEFT_BRACE);
    EXPECT_EQ(tokens[idx++].type, PRINT);
    EXPECT_EQ(tokens[idx].type, IDENTIFIER);
    EXPECT_EQ(tokens[idx].line, 2);
    EXPECT_EQ(tokens[idx++].lexeme, "a");
    EXPECT_EQ(tokens[idx++].type, SEMICOLON);
    EXPECT_EQ(tokens[idx++].type, RIGHT_BRACE);
    EXPECT_EQ(tokens[idx++].type, ELSE);
    EXPECT_EQ(tokens[idx++].type, LEFT_BRACE);
    EXPECT_EQ(tokens[idx++].type, IF);
    EXPECT_EQ(tokens[idx++].type, LEFT_PAREN);
    EXPECT_EQ(tokens[idx++].type, FALSE);
    EXPECT_EQ(tokens[idx++].type, RIGHT_PAREN);
    EXPECT_EQ(tokens[idx++].type, PRINT);
    EXPECT_EQ(tokens[idx].type, IDENTIFIER);
    EXPECT_EQ(tokens[idx++].lexeme, "b");
    EXPECT_EQ(tokens[idx].line, 5);
    EXPECT_EQ(tokens[idx++].type, SEMICOLON);
    EXPECT_EQ(tokens[idx++].type, ELSE);
    EXPECT_EQ(tokens[idx++].type, LEFT_BRACE);
    EXPECT_EQ(tokens[idx++].type, PRINT);
    EXPECT_EQ(tokens[idx].type, IDENTIFIER);
    EXPECT_EQ(tokens[idx++].lexeme, "c");
    EXPECT_EQ(tokens[idx].line, 7);
    EXPECT_EQ(tokens[idx++].type, SEMICOLON);
    EXPECT_EQ(tokens[idx++].type, RIGHT_BRACE);

    EXPECT_EQ(tokens[24].type, EOF_);
}

TEST_F(ScannerTest, WhileLoop) {
    string src = "\
        var a = 5;\n\
        while (a <= 10) {\n\
            a = a + 1;\n\
        }\n";

    Scanner scanner(src);
    const vector<Token>& tokens = scanner.scanTokens();
    
    EXPECT_EQ(tokens.size(), 20);
    int idx = 0;
    EXPECT_EQ(tokens[idx++].type, VAR);
    EXPECT_EQ(tokens[idx++].type, IDENTIFIER);
    EXPECT_EQ(tokens[idx++].type, EQUAL);
    EXPECT_EQ(tokens[idx++].type, INTEGER);
    EXPECT_EQ(tokens[idx++].type, SEMICOLON);
    EXPECT_EQ(tokens[idx++].type, WHILE);
    EXPECT_EQ(tokens[idx++].type, LEFT_PAREN);
    EXPECT_EQ(tokens[idx++].type, IDENTIFIER);
    EXPECT_EQ(tokens[idx++].type, LESS_EQUAL);
    EXPECT_EQ(tokens[idx++].type, INTEGER);
    EXPECT_EQ(tokens[idx++].type, RIGHT_PAREN);
    EXPECT_EQ(tokens[idx++].type, LEFT_BRACE);
    EXPECT_EQ(tokens[idx++].type, IDENTIFIER);
    EXPECT_EQ(tokens[idx++].type, EQUAL);
    EXPECT_EQ(tokens[idx++].type, IDENTIFIER);
    EXPECT_EQ(tokens[idx++].type, PLUS);
    EXPECT_EQ(tokens[idx++].type, INTEGER);
    EXPECT_EQ(tokens[idx++].type, SEMICOLON);
    EXPECT_EQ(tokens[idx++].type, RIGHT_BRACE);
    EXPECT_EQ(tokens[19].type, EOF_);
}

TEST_F(ScannerTest, ForLoop) {
    string src = "\
        for (var a = 10; a >= 5; a = a - 1) {\n\
            print a;\n\
        }\n";

    Scanner scanner(src);
    const vector<Token>& tokens = scanner.scanTokens();
    
    EXPECT_EQ(tokens.size(), 23);
    int idx = 0;
    EXPECT_EQ(tokens[idx++].type, FOR);
    EXPECT_EQ(tokens[idx++].type, LEFT_PAREN);
    EXPECT_EQ(tokens[idx++].type, VAR);
    EXPECT_EQ(tokens[idx++].type, IDENTIFIER);
    EXPECT_EQ(tokens[idx++].type, EQUAL);
    EXPECT_EQ(tokens[idx++].type, INTEGER);
    EXPECT_EQ(tokens[idx++].type, SEMICOLON);
    EXPECT_EQ(tokens[idx++].type, IDENTIFIER);
    EXPECT_EQ(tokens[idx++].type, GREATER_EQUAL);
    EXPECT_EQ(tokens[idx++].type, INTEGER);
    EXPECT_EQ(tokens[idx++].type, SEMICOLON);
    EXPECT_EQ(tokens[idx++].type, IDENTIFIER);
    EXPECT_EQ(tokens[idx++].type, EQUAL);
    EXPECT_EQ(tokens[idx++].type, IDENTIFIER);
    EXPECT_EQ(tokens[idx++].type, MINUS);
    EXPECT_EQ(tokens[idx++].type, INTEGER);
    EXPECT_EQ(tokens[idx++].type, RIGHT_PAREN);
    EXPECT_EQ(tokens[idx++].type, LEFT_BRACE);
    EXPECT_EQ(tokens[idx++].type, PRINT);
    EXPECT_EQ(tokens[idx++].type, IDENTIFIER);
    EXPECT_EQ(tokens[idx++].type, SEMICOLON);
    EXPECT_EQ(tokens[idx++].type, RIGHT_BRACE);
    EXPECT_EQ(tokens[idx++].type, EOF_);
}

TEST_F(ScannerTest, Function) {
    string src = "\
        fun sayHi(first, last) {\n\
            print \"Hi, \" + first + \" \" + last + \"!\";\n\
        }\n\
        \n\
        sayHi(\"Dear\", \"Reader\");\n\
    ";

    Scanner scanner(src);
    const vector<Token>& tokens = scanner.scanTokens();
    
    EXPECT_EQ(tokens.size(), 28);
    int idx = 0;
    EXPECT_EQ(tokens[idx++].type, FUN);
    EXPECT_EQ(tokens[idx++].type, IDENTIFIER);
    EXPECT_EQ(tokens[idx++].type, LEFT_PAREN);
    EXPECT_EQ(tokens[idx++].type, IDENTIFIER);
    EXPECT_EQ(tokens[idx++].type, COMMA);
    EXPECT_EQ(tokens[idx++].type, IDENTIFIER);
    EXPECT_EQ(tokens[idx++].type, RIGHT_PAREN);
    EXPECT_EQ(tokens[idx++].type, LEFT_BRACE);
    EXPECT_EQ(tokens[idx++].type, PRINT);
    EXPECT_EQ(tokens[idx++].type, STRING);
    EXPECT_EQ(tokens[idx++].type, PLUS);
    EXPECT_EQ(tokens[idx++].type, IDENTIFIER);
    EXPECT_EQ(tokens[idx++].type, PLUS);
    EXPECT_EQ(tokens[idx++].type, STRING);
    EXPECT_EQ(tokens[idx++].type, PLUS);
    EXPECT_EQ(tokens[idx++].type, IDENTIFIER);
    EXPECT_EQ(tokens[idx++].type, PLUS);
    EXPECT_EQ(tokens[idx++].type, STRING);
    EXPECT_EQ(tokens[idx++].type, SEMICOLON);
    EXPECT_EQ(tokens[idx++].type, RIGHT_BRACE);
    EXPECT_EQ(tokens[idx++].type, IDENTIFIER);
    EXPECT_EQ(tokens[idx++].type, LEFT_PAREN);
    EXPECT_EQ(tokens[idx++].type, STRING);
    EXPECT_EQ(tokens[idx++].type, COMMA);
    EXPECT_EQ(tokens[idx++].type, STRING);
    EXPECT_EQ(tokens[idx++].type, RIGHT_PAREN);
    EXPECT_EQ(tokens[idx++].type, SEMICOLON);
    EXPECT_EQ(tokens[idx++].type, EOF_);

}