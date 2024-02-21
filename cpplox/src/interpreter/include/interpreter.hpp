#pragma once

#include <expr.hpp>
#include <stmt.hpp>
#include "environment.hpp"

namespace cpplox {

    class Interpreter : public Expr::Visitor, public Stmt::Visitor {
    public:
        Interpreter();
        ~Interpreter();

        void interpret(Expr* expr);
        any interpret(vector<Stmt*> stmts);

        any visitLiteralExpr(LiteralExpr* expr) override;
        any visitBinaryExpr(BinaryExpr* expr) override;
        any visitGroupingExpr(GroupingExpr* expr) override;
        any visitUnaryExpr(UnaryExpr* expr) override;
        any visitCallExpr(CallExpr* expr) override;
        any visitExpressionStmt(ExpressionStmt* stmt) override;
        any visitPrintStmt(PrintStmt* stmt) override;
        any visitVariableExpr(VariableExpr* expr) override;
        any visitVarStmt(VarStmt* stmt) override;
        any visitAssignExpr(AssignExpr* expr) override;
        any visitBlockStmt(BlockStmt* stmt) override;
        any visitIfStmt(IfStmt* stmt) override;
        any visitWhileStmt(WhileStmt* stmt) override;
        any visitFunctionStmt(FunctionStmt* stmt) override;
        any visitReturnStmt(ReturnStmt* stmt) override;

        any executeBlock(vector<Stmt*> stmts, Environment* enclosing);

    private:
        any evaluate(Expr* expr);
        any execute(Stmt* stmt);
        bool isTruthy(any val);
        bool isEqual(any a, any b);
        string stringify(const any& value);

        Environment* environment;
    };

}