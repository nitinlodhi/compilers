#pragma once

#include <lox.hpp>
#include <token.hpp>

namespace cpplox {

    class Scanner {
    public:
        Scanner(const string& _source);
        vector<Token>& scanTokens();

    private:
        void scanToken();
        bool isAtEnd();
        void addToken(TokenType type);
        void addToken(TokenType type, any obj);
        bool match(char expected);
        char peek();
        char peekNext();
        char advance();

        void string_();
        void number();
        void identifier();

        int line;
        int current;
        int start;

        string source;
        vector<Token> tokens;
    };
}  // namespace cpplox
