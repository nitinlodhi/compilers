#include <parser.hpp>

namespace cpplox {

    Parser::Parser(vector<Token>& _tokens)
        : tokens(_tokens) {
    }

    // Expr* parse() {
    //     try {
    //         return expression();
    //     } catch (ParseError* err) {
    //         return nullptr;
    //     }
    // }

    vector<Stmt*> Parser::parse() {
        vector<Stmt*> statements;
        while (!isAtEnd()) {
            // statements.push_back(statement());
            statements.push_back(declaration());
        }
        return statements;
    }

    Expr* Parser::expression() {
        return assignment();
    }

    Expr* Parser::equality() {
        Expr* expr = comparison();

        while (match({BANG_EQUAL, EQUAL_EQUAL})) {
            auto token = previous();
            Expr* right = comparison();
            expr = new BinaryExpr(expr, token, right);
        }
        return expr;
    }

    Expr* Parser::comparison() {
        Expr* expr = term();

        while (match({GREATER, GREATER_EQUAL,
                      LESS, LESS_EQUAL})) {
            auto token = previous();
            Expr* right = term();
            expr = new BinaryExpr(expr, token, right);
        }
        return expr;
    }

    Expr* Parser::term() {
        auto expr = factor();
        while (match({MINUS, PLUS})) {
            auto token = previous();
            auto right = factor();
            expr = new BinaryExpr(expr, token, right);
        }
        return expr;
    }

    Expr* Parser::factor() {
        auto expr = unary();
        while (match({SLASH, STAR})) {
            auto token = previous();
            auto right = unary();
            expr = new BinaryExpr(expr, token, right);
        }
        return expr;
    }

    Expr* Parser::unary() {
        if (match({BANG, MINUS})) {
            Token* token = previous();
            auto expr = primary();
            return new UnaryExpr(token, expr);
        }
        return call();
    }

    Expr* Parser::call() {
        Expr* expr = primary();
        while (true) {
            if (match({LEFT_PAREN})) {
                expr = finishCall(expr);
            } else
                break;
        }
        return expr;
    }

    Expr* Parser::finishCall(Expr* callee) {
        vector<Expr*> args;
        if (!check(RIGHT_PAREN)) {
            do {
                if (args.size() >= 255)
                    error(peek(), "Can't have more than 255 arguments.");
                args.push_back(expression());
            } while (match({COMMA}));
        }
        Token* paren = consume(RIGHT_PAREN, "Expect ')' after arguments.");
        return new CallExpr(callee, paren, args);
    }

    Expr* Parser::primary() {
        if (match({INTEGER, DOUBLE, STRING})) {
            auto token = previous();
            // std::cout << " -" << std::any_cast<string>(token->literal) << "-" << std::endl;
            switch (token->type) {
            case INTEGER:
                return new LiteralExpr(token->literal);
            case DOUBLE:
                return new LiteralExpr(token->literal);
            case STRING:
                return new LiteralExpr(token->literal);
            }
        }

        if (match({TRUE})) {
            return new LiteralExpr(true);
        }

        if (match({FALSE})) {
            return new LiteralExpr(false);
        }

        if (match({NIL})) {
            return new LiteralExpr(nullptr);
        }

        if (match({IDENTIFIER})) {
            return new VariableExpr(previous());
        }

        if (match({LEFT_PAREN})) {
            Expr* expr = expression();
            consume(RIGHT_PAREN, "Expect ')' after expression.");
            return new GroupingExpr(expr);
        }

        throw error(peek(), "Expect expression.");
    }

    Expr* Parser::assignment() {
        Expr* expr = equality();

        if (match({EQUAL})) {
            Token* equals = previous();
            Expr* value = assignment();
            if (dynamic_cast<VariableExpr*>(expr) != nullptr) {
                Token* name = (dynamic_cast<VariableExpr*>(expr))->name;
                // TODO
                delete expr;
                return new AssignExpr(name, value);
            }
            error(equals, "Invalid assignment target.");
        }
        return expr;
    }

    Stmt* Parser::statement() {
        if (match({IF}))
            return ifStatement();
        if (match({FOR}))
            return forStatement();
        if (match({WHILE}))
            return whileStatement();
        if (match({PRINT}))
            return printStatement();
        if (match({RETURN}))
            return returnStatement();
        if (match({LEFT_BRACE}))
            return new BlockStmt(block());
        return expressionStatement();
    }

    Stmt* Parser::returnStatement() {
        Token* keyword = previous();
        Expr* value = nullptr;
        if (!check(SEMICOLON)) {
            value = expression();
        }
        consume(SEMICOLON, "Expect ';' after return value.");
        return new ReturnStmt(keyword, value);
    }

    Stmt* Parser::printStatement() {
        Expr* expr = expression();
        consume(SEMICOLON, "Expect ';' after value.");
        return new PrintStmt(expr);
    }

    Stmt* Parser::expressionStatement() {
        Expr* expr = expression();
        consume(SEMICOLON, "Expect ';' after value.");
        return new ExpressionStmt(expr);
    }

    Stmt* Parser::ifStatement() {
        consume(LEFT_PAREN, "Expect '(' after 'if'.");
        Expr* condition = expression();
        consume(RIGHT_PAREN, "Expect ')' after if condition.");

        Stmt* thenBranch = statement();
        Stmt* elseBranch = nullptr;
        if (match({ELSE})) {
            elseBranch = statement();
        }
        return new IfStmt(condition, thenBranch, elseBranch);
    }

    Stmt* Parser::whileStatement() {
        consume(LEFT_PAREN, "Expect '(' after 'while'.");
        Expr* condition = expression();
        consume(RIGHT_PAREN, "Expect ')' after condition.");

        Stmt* body = statement();
        return new WhileStmt(condition, body);
    }

    Stmt* Parser::forStatement() {
        consume(LEFT_PAREN, "Expect '(' after 'for'.");
        Stmt* initializer = nullptr;
        if (match({VAR})) {
            initializer = varDeclaration();
        } else if (match({SEMICOLON})) {
            initializer = nullptr;
        } else {
            initializer = expressionStatement();
        }

        Expr* condition = nullptr;
        if (!check(SEMICOLON)) {
            condition = expression();
        }
        consume(SEMICOLON, "Expect ';' after loop condition.");

        Expr* incr = nullptr;
        if (!check(RIGHT_PAREN)) {
            incr = expression();
        }
        consume(RIGHT_PAREN, "Expect ')' after for clauses.");

        Stmt* body = statement();

        if (incr) {
            auto temp = body;
            body = new BlockStmt({body, new ExpressionStmt(incr)});
            // TODO
            // delete temp;
        }
        if (!condition) {
            condition = new LiteralExpr(true);
        }
        body = new WhileStmt(condition, body);

        if (initializer) {
            body = new BlockStmt({initializer, body});
        }
        return body;
    }

    FunctionStmt* Parser::function(string kind) {
        Token* name = consume(IDENTIFIER, "Expect " + kind + " name.");
        consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
        vector<Token*> params;
        if (!check(RIGHT_PAREN)) {
            do {
                if (params.size() >= 255)
                    error(peek(), "Can't have more than 255 parameters.");
                params.push_back(consume(IDENTIFIER, "Expect parameter name."));
            } while (match({COMMA}));
        }
        consume(RIGHT_PAREN, "Expect ')' after parameters.");

        consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
        vector<Stmt*> body = block();
        // auto body = new BlockStmt(block());
        return new FunctionStmt(name, params, body);
    }

    vector<Stmt*> Parser::block() {
        vector<Stmt*> statements;
        while (!check(RIGHT_BRACE) && !isAtEnd()) {
            statements.push_back(declaration());
        }
        consume(RIGHT_BRACE, "Expect '}' after block,");
        return statements;
    }

    Stmt* Parser::varDeclaration() {
        auto name = consume(IDENTIFIER, "Expect variable name.");

        Expr* initializer = nullptr;
        if (match({EQUAL})) {
            initializer = expression();
        }
        consume(SEMICOLON, "Expect ';' after variable declaration.");
        return new VarStmt(name, initializer);
    }

    Stmt* Parser::declaration() {
        if (match({FUN})) {
            return function("function");
        }
        if (match({VAR})) {
            return varDeclaration();
        }
        return statement();
    }

    Token* Parser::consume(TokenType type, const string& message) {
        if (check(type))
            return advance();
        throw error(peek(), message);
    }

    bool Parser::match(const vector<TokenType>& types) {
        for (auto type : types) {
            if (check(type)) {
                advance();
                return true;
            }
        }
        return false;
    }

    Token* Parser::advance() {
        if (!isAtEnd())
            current++;
        return previous();
    }

    bool Parser::check(TokenType type) {
        if (isAtEnd())
            return false;
        return type == tokens[current].type;
    }

    bool Parser::isAtEnd() {
        return current >= tokens.size() || peek()->type == EOF_;
    }

    Token* Parser::peek() {
        return &tokens[current];
    }

    Token* Parser::previous() {
        return const_cast<Token*>(&tokens[current - 1]);
    }

    Parser::ParseError* Parser::error(Token* token, string message) {
        Lox::error(token, message);
        return new ParseError(message);
    }

    void Parser::synchronize() {
        advance();

        while (!isAtEnd()) {
            if (previous()->type == SEMICOLON)
                return;

            switch (peek()->type) {
            case CLASS:
            case FUN:
            case VAR:
            case FOR:
            case IF:
            case WHILE:
            case PRINT:
            case RETURN:
                return;
            }

            advance();
        }
    }

}