#pragma once

#include <common.hpp>

enum TokenType {
    // Single-character tokens.
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    // One or two character tokens.
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Literals.
    IDENTIFIER,
    STRING,
    INTEGER,
    DOUBLE,

    // Keywords.
    AND,
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,

    EOF_
};

extern const string tokenNames[];
extern const std::map<std::string, TokenType> keywords;

class Token {
public:
    Token(TokenType _type, string _lexeme, any _obj, int _line)
        : type(_type), lexeme(_lexeme), literal(_obj), line(_line) {}

    ~Token() {
        // delete literal;
    }

    friend std::ostream& operator<<(std::ostream& os, const Token& token);

    string toString() {
        auto idx = static_cast<int>(type);
        string info = tokenNames[idx] + " " + lexeme;
        // if (literal != nullptr)
        //     info += " " + literal->toString();
        return info;
        // return tokenNames[idx] + " " + lexeme + (literal != nullptr) ? " " + literal->toString() : "";
    }

    static Token* create(TokenType type,
                         string lexeme,
                         const any obj,
                         int line) {
        return new Token(type, lexeme, obj, line);
    }

    // private:
    const TokenType type;
    const string lexeme;
    const any literal;
    const int line;
};
