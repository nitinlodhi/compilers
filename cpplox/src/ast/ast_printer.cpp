#include <cassert>
#include <iostream>
#include <ast_printer.hpp>
#include <sstream>

#include <nlohmann/json.hpp>

void AstPrinter::print(vector<Stmt*>& stmts) {
    for (auto& stmt : stmts) {
        std::cout << any_cast<nlohmann::ordered_json>((stmt->accept(this))).dump(4) << std::endl;
    }
}

any AstPrinter::visitLiteralExpr(LiteralExpr* expr) {
    nlohmann::ordered_json json;
    if (expr->value.type() == typeid(int))
        json["Literal"] = any_cast<int>(expr->value);
    else if (expr->value.type() == typeid(string))
        json["Literal"] = any_cast<string>(expr->value);
    else if (expr->value.type() == typeid(double))
        json["Literal"] = any_cast<double>(expr->value);
    else if (expr->value.type() == typeid(bool))
        json["Literal"] = any_cast<bool>(expr->value);
    else if (expr->value.type() == typeid(nullptr))
        json["Literal"] = nullptr;
    // json.dump(4);
    return json;
}

any AstPrinter::visitBinaryExpr(BinaryExpr* expr) {
    nlohmann::ordered_json json;
    json["BinaryExpression"]["left"] = any_cast<nlohmann::ordered_json>(expr->left->accept(this));
    json["BinaryExpression"]["operator"] = expr->op->lexeme;
    json["BinaryExpression"]["right"] = any_cast<nlohmann::ordered_json>(expr->right->accept(this));
    return json;
}

any AstPrinter::visitGroupingExpr(GroupingExpr* expr) {
    nlohmann::ordered_json json;
    json["GroupingExpression"]["value"] = any_cast<nlohmann::ordered_json>(expr->expr->accept(this));

    return json;
}

any AstPrinter::visitUnaryExpr(UnaryExpr* expr) {
    nlohmann::ordered_json json;
    json["UnaryExpression"]["operator"] = expr->op->lexeme;
    json["UnaryExpression"]["right"] = any_cast<nlohmann::ordered_json>(expr->right->accept(this));
    return json;
}

any AstPrinter::visitCallExpr(CallExpr* expr) {
    nlohmann::ordered_json json;
    json["CallExpression"]["callee"] = any_cast<nlohmann::ordered_json>(expr->callee->accept(this));
    for (auto& arg : expr->arguments) {
        json["CallExpression"]["arguments"].push_back(any_cast<nlohmann::ordered_json>(arg->accept(this)));
    }
    return json;
}

any AstPrinter::visitExpressionStmt(ExpressionStmt* stmt) {
    nlohmann::ordered_json json;
    json["ExpressionStatement"] = any_cast<nlohmann::ordered_json>(stmt->expression->accept(this));
    return json;
}

any AstPrinter::visitPrintStmt(PrintStmt* stmt) {
    nlohmann::ordered_json json;
    json["PrintStatement"]["expression"] = any_cast<nlohmann::ordered_json>(stmt->expression->accept(this));
    return json;
}

any AstPrinter::visitVariableExpr(VariableExpr* expr) {
    nlohmann::ordered_json json;
    json["VariableExpression"]["name"] = expr->name->lexeme;
    return json;
}

any AstPrinter::visitVarStmt(VarStmt* stmt) {
    nlohmann::ordered_json json;
    json["VariableDeclaration"]["name"] = stmt->name->lexeme;
    if (stmt->initializer)
        json["VariableDeclaration"]["initializer"] = any_cast<nlohmann::ordered_json>(stmt->initializer->accept(this));
    else
        json["VariableDeclaration"]["initializer"] = nullptr;
    return json;
}

any AstPrinter::visitAssignExpr(AssignExpr* expr) {
    nlohmann::ordered_json json;
    json["AssignmentExpression"]["name"] = expr->name->lexeme;
    json["AssignmentExpression"]["value"] = any_cast<nlohmann::ordered_json>(expr->value->accept(this));
    return json;
}

any AstPrinter::visitBlockStmt(BlockStmt* stmt) {
    nlohmann::ordered_json json;
    for (auto& stmt : stmt->statements) {
        json["BlockStatement"]["body"].push_back(any_cast<nlohmann::ordered_json>(stmt->accept(this)));
    }
    return json;
}

any AstPrinter::visitIfStmt(IfStmt* stmt) {
    nlohmann::ordered_json json;
    json["IfStatement"]["test"] = any_cast<nlohmann::ordered_json>(stmt->condition->accept(this));
    json["IfStatement"]["consequent"] = any_cast<nlohmann::ordered_json>(stmt->thenBranch->accept(this));
    if (stmt->elseBranch)
        json["IfStatement"]["alternate"] = any_cast<nlohmann::ordered_json>(stmt->elseBranch->accept(this));
    else
        json["IfStatement"]["alternate"] = nullptr;
    return json;
}

any AstPrinter::visitWhileStmt(WhileStmt* stmt) {
    nlohmann::ordered_json json;
    json["WhileStatement"]["test"] = any_cast<nlohmann::ordered_json>(stmt->condition->accept(this));
    json["WhileStatement"]["body"] = any_cast<nlohmann::ordered_json>(stmt->body->accept(this));
    return json;
}

any AstPrinter::visitFunctionStmt(FunctionStmt* stmt) {
    nlohmann::ordered_json json;
    json["FunctionDeclaration"]["name"] = stmt->name->lexeme;
    for (auto& param: stmt->params)
        json["FunctionDeclaration"]["params"].push_back(param->lexeme);
    for (auto& stmt: stmt->body)
        json["FunctionDeclaration"]["body"].push_back(any_cast<nlohmann::ordered_json>(stmt->accept(this)));
    return json;
}

any AstPrinter::visitReturnStmt(ReturnStmt* stmt) {
    nlohmann::ordered_json json;
    json["ReturnStatement"]["value"] = any_cast<nlohmann::ordered_json>(stmt->value->accept(this));
    return json;
}