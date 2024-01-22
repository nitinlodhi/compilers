#pragma once

#include <stmt.hpp>
#include <lox.hpp>

namespace cpplox {

    class Parser {
    public:
        Parser(vector<Token>& _tokens);
        vector<Stmt*> parse();

        class ParseError : public std::runtime_error {
        public:
            ParseError(string error)
                : std::runtime_error(error) {}
        };

    private:
        Expr* expression();
        Expr* equality();
        Expr* comparison();
        Expr* term();
        Expr* factor();
        Expr* unary();
        Expr* call();
        Expr* primary();
        Expr* assignment();

        Expr* finishCall(Expr* callee);

        Stmt* statement();
        Stmt* printStatement();
        Stmt* expressionStatement();
        Stmt* ifStatement();
        Stmt* whileStatement();
        Stmt* forStatement();
        Stmt* returnStatement();
        FunctionStmt* function(string kind);

        vector<Stmt*> block();
        Stmt* varDeclaration();
        Stmt* declaration();

        Token* consume(TokenType type, const string& message);
        bool match(const vector<TokenType>& types);
        Token* advance();
        bool check(TokenType type);
        bool isAtEnd();
        Token* peek();
        Token* previous();
        ParseError* error(Token* token, string message);
        void synchronize();

        vector<Token>& tokens;
        int current = 0;
    };

}