#include "interpreter.hpp"
#include <lox_function.hpp>
#include <return.hpp>

#include <cassert>

namespace cpplox {

    Interpreter::Interpreter() {
        environment = new Environment();
    }

    Interpreter::~Interpreter() {
        delete environment;
    }

    any Interpreter::visitLiteralExpr(LiteralExpr* expr) {
        return expr->value;
    }

    any Interpreter::visitBinaryExpr(BinaryExpr* expr) {
        any left = evaluate(expr->left);
        any right = evaluate(expr->right);

        switch (expr->op->type) {
        case PLUS: {
            if (left.type() == typeid(int) && right.type() == typeid(int)) {
                return any_cast<int>(left) + any_cast<int>(right);
            } else if (left.type() == typeid(string) && right.type() == typeid(string)) {
                return any_cast<string>(left) + any_cast<string>(right);
            }
        } break;
        case MINUS: {
            if (left.type() == typeid(int) && right.type() == typeid(int)) {
                return any_cast<int>(left) - any_cast<int>(right);
            }
        } break;
        case STAR: {
            if (left.type() == typeid(int) && right.type() == typeid(int)) {
                return any_cast<int>(left) * any_cast<int>(right);
            }
        } break;
        case SLASH: {
            if (left.type() == typeid(int) && right.type() == typeid(int)) {
                return any_cast<int>(left) / any_cast<int>(right);
            }
        } break;
        case GREATER:
            return any_cast<int>(left) > any_cast<int>(right);
        case GREATER_EQUAL:
            return any_cast<int>(left) >= any_cast<int>(right);
        case LESS:
            return any_cast<int>(left) < any_cast<int>(right);
        case LESS_EQUAL:
            return any_cast<int>(left) <= any_cast<int>(right);
        case BANG_EQUAL:
            return !isEqual(left, right);
        case EQUAL_EQUAL:
            return isEqual(left, right);
        }

        return nullptr;
    }

    any Interpreter::visitGroupingExpr(GroupingExpr* expr) {
        return evaluate(expr);
    }

    any Interpreter::visitUnaryExpr(UnaryExpr* expr) {
        std::any right = evaluate(expr->right);
        switch (expr->op->type) {
        case MINUS: {
            if (right.type() == typeid(int))
                return -1 * any_cast<int>(right);
            else if (right.type() == typeid(double))
                return -1 * any_cast<double>(right);
        } break;
        case BANG: {
            return !isTruthy(right);
        } break;
        }

        return nullptr;
    }

    any Interpreter::visitCallExpr(CallExpr* expr) {
        LoxCallable* fn = any_cast<LoxFunction*>(evaluate(expr->callee));
        vector<any> args;
        for (auto& e : expr->arguments)
            args.push_back(evaluate(e));

        // LoxFunction* fn = any_cast<LoxFunction*>(callee);
        // if (arguments.size() != function.arity()) {
        //     throw new RuntimeError(expr.paren, "Expected " +
        //                                            function.arity() + " arguments but got " +
        //                                            arguments.size() + ".");
        // }
        return fn->call(this, args);
    }

    any Interpreter::visitReturnStmt(ReturnStmt* stmt) {
        any value;
        if (stmt->value != nullptr) {
            value = evaluate(stmt->value);
            throw Return(value);
        }
        return nullptr;
    }

    any Interpreter::visitExpressionStmt(ExpressionStmt* stmt) {
        return evaluate(stmt->expression);
    }

    any Interpreter::visitPrintStmt(PrintStmt* stmt) {
        auto v = evaluate(stmt->expression);
        std::cout << stringify(v) << std::endl;
        return v;
    }

    any Interpreter::visitVariableExpr(VariableExpr* expr) {
        return environment->get(expr->name);
    }

    any Interpreter::visitVarStmt(VarStmt* stmt) {
        any val = nullptr;
        if (stmt->initializer != nullptr) {
            val = evaluate(stmt->initializer);
        }
        environment->define(stmt->name->lexeme, val);
        return val;
    }

    any Interpreter::visitAssignExpr(AssignExpr* expr) {
        auto value = evaluate(expr->value);
        environment->assign(expr->name, value);
        return value;
    }

    any Interpreter::visitBlockStmt(BlockStmt* stmt) {
        return executeBlock(stmt->statements, new Environment(environment));
    }

    any Interpreter::visitIfStmt(IfStmt* stmt) {
        if (isTruthy(evaluate(stmt->condition))) {
            return execute(stmt->thenBranch);
        } else if (stmt->elseBranch != nullptr) {
            return execute(stmt->elseBranch);
        }
        return nullptr;
    }

    any Interpreter::visitWhileStmt(WhileStmt* stmt) {
        any res = nullptr;
        while (isTruthy(evaluate(stmt->condition))) {
            res = execute(stmt->body);
        }
        return res;
    }

    any Interpreter::visitFunctionStmt(FunctionStmt* stmt) {
        LoxFunction* loxFunc = new LoxFunction(stmt);
        environment->define(stmt->name->lexeme, loxFunc);
        return nullptr;
    }

    any Interpreter::evaluate(Expr* expr) {
        return expr->accept(this);
    }

    any Interpreter::execute(Stmt* stmt) {
        return stmt->accept(this);
    }

    any Interpreter::executeBlock(vector<Stmt*> stmts, Environment* enclosing) {
        any val = nullptr;
        Environment* prev = environment;
        environment = enclosing;
        for (auto stmt : stmts) {
            val = execute(stmt);
        }
        environment = prev;
        delete enclosing;
        return val;
    }

    bool Interpreter::isTruthy(any val) {
        if (typeid(nullptr) == val.type())
            return true;
        if (typeid(bool) == val.type())
            return any_cast<bool>(val);
        return true;
    }

    bool Interpreter::isEqual(any a, any b) {
        if (a.type() == b.type()) {
            if (a.type() == typeid(nullptr) && (b.type() == typeid(nullptr)))
                return true;
            if (a.type() == typeid(nullptr))
                return false;
            if (a.type() == typeid(int))
                return any_cast<int>(a) == any_cast<int>(b);
            if (a.type() == typeid(double))
                return any_cast<double>(a) == any_cast<double>(b);
            if (a.type() == typeid(string))
                return any_cast<string>(a) == any_cast<string>(b);
        }
        return false;
    }

    string Interpreter::stringify(const any& value) {
        if (value.type() == typeid(int)) {
            return std::to_string(any_cast<int>(value));
        } else if (value.type() == typeid(double)) {
            return std::to_string(any_cast<double>(value));
        } else if (value.type() == typeid(bool))
            return std::to_string(any_cast<bool>(value));
        else {
            return any_cast<string>(value);
        }
    }

    void Interpreter::interpret(Expr* expr) {
        any result = evaluate(expr);
        if (result.type() == typeid(int)) {
            std::cout << "Integer: "
                      << std::any_cast<int>(result)
                      << std::endl;
        } else if (result.type() == typeid(string)) {
            std::cout << "String: "
                      << std::any_cast<string>(result)
                      << std::endl;
        } else if (result.type() == typeid(true)) {
            std::cout << "Boolean: "
                      << "true"
                      << std::endl;
        } else if (result.type() == typeid(false)) {
            std::cout << "Boolean: "
                      << "false"
                      << std::endl;
        } else if (result.type() == typeid(nullptr)) {
            std::cout << "Nil: "
                      << "nullptr"
                      << std::endl;
        }
    }

    any Interpreter::interpret(vector<Stmt*> stmts) {
        any result = nullptr;
        for (auto& stmt : stmts)
            result = execute(stmt);
        for (auto _x : stmts)
            delete _x;
        return result;
    }

}