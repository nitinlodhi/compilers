#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"

typedef struct {
    Token current;
    Token previous;
    bool hadError;
    bool panicMode;
} Parser;

typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT,  // =
    PREC_OR,          // or
    PREC_AND,         // and
    PREC_EQUALITY,    // == !=
    PREC_COMPARISON,  // < > <= >=
    PREC_TERM,        // + -
    PREC_FACTOR,      // * /
    PREC_UNARY,       // ! -
    PREC_CALL,        // . ()
    PREC_PRIMARY
} Precedence;

Parser parser;
Chunk* compilingChunk;

static Chunk* currentChunk() {
    return compilingChunk;
}

static void errorAt(Token* token, const char* message) {
    if (parser.panicMode)
        return;
    parser.panicMode = true;
    fprintf(stderr, "[line %d] Error", token->line);

    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if (token->type == TOKEN_ERROR) {
        // Nothing
    } else {
        fprintf(stderr, "at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
}

static void errorAtCurrent(const char* message) {
    errorAt(&parser.current, message);
}

static void error(const char* message) {
    errorAt(&parser.previous, message);
}

static void advance() {
    parser.previous = parser.current;

    for (;;) {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR)
            break;
        errorAtCurrent(parser.current.start);
    }
}

static void consume(TokenType type, const char* message) {
    if (parser.current.type == type) {
        advance();
        return;
    }

    errorAtCurrent(message);
}

static void emitByte(uint8_t byte) {
    writeChunk(currentChunk(), byte, parser.previous.line);
}

static void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

static void emitReturn() {
    emitByte(OP_RETURN);
}

static void endCompiler() {
    emitReturn();
}

static void expression() {
    // What goes here?
    parsePrecedence(PREC_ASSIGNMENT);
}

static uint8_t makeConstant(Value val) {
    int idx = addConstant(currentChunk(), val);
    if (idx > UINT8_MAX) {
        error("Too many constants in one chunk.");
        return 0;
    }
    return (uint8_t)idx;
}

static void emitConstant(Value val) {
    emitBytes(OP_CONSTANT, makeConstant(val));
}

static void number() {
    double val = strtod(parser.previous.start, NULL);
    emitConstant(val);
}

static void grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression");
}

static void unary() {
    TokenType opType = parser.previous.type;

    // expression();
    parsePrecedence(PREC_UNARY);

    switch (opType) {
    case TOKEN_MINUS:
        emitByte(OP_NEGATE);
        break;
    default:
        return;
    }
}

static void parsePrecedence(Precedence precedence) {
}

bool compile(const char* source, Chunk* chunk) {
    initScanner(source);
    compilingChunk = chunk;

    parser.hadError = false;
    parser.panicMode = false;
#if 1
    int line = -1;
    for (;;) {
        Token token = scanToken();
        if (token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        } else {
            printf("   | ");
        }
        printf("%2d '%.*s'\n", token.type, token.length, token.start);
        if (token.type == TOKEN_EOF)
            break;
    }
#endif
#if 0
    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression.");
    endCompiler();
#endif
    return !parser.hadError;
}