#pragma once

#include <sstream>

#include <expr.hpp>

class AstPrinter : public Expr::Visitor {
public:
    string print(Expr* expr) {
        const auto& str = expr->accept(this);
        return std::any_cast<string>(str);
    }

    any visitBinaryExpr(BinaryExpr* expr) {
        return parenthesize(expr->op->lexeme, {expr->left, expr->right});
    }

    any visitGroupingExpr(GroupingExpr* grouping) {
        return parenthesize("group", {grouping->expr});
    }
    any visitLiteralExpr(LiteralExpr* literal) {
        return literal->value;
    }
    any visitUnaryExpr(UnaryExpr* unary) {
        return parenthesize(unary->op->lexeme, {unary->right});
    }

    string parenthesize(const string& name, const vector<Expr*>& exprs) {
        std::stringstream ss;
        ss << "(" << name;
        for (auto expr : exprs) {
            ss << " ";
            auto r = expr->accept(this);
            if (r.type() == typeid(int)) {
                ss << std::any_cast<int>(expr->accept(this));
            } else if (r.type() == typeid(string)) {
                ss << std::any_cast<string>(expr->accept(this));
            }
        }
        ss << ")";
        return ss.str();
    }
};