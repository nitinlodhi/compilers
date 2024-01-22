#pragma once

#include <stmt.hpp>
#include <nlohmann/json.hpp>

class AstPrinter : public Expr::Visitor, public Stmt::Visitor {
public:
    void print(vector<Stmt*>& stmts);
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

private:
};