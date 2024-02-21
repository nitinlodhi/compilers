#pragma once

#include <expr.hpp>

class BlockStmt;
class ExpressionStmt;
class FunctionStmt;
class IfStmt;
class WhileStmt;
class PrintStmt;
class ReturnStmt;
class VarStmt;

class Stmt {
public:
	class Visitor {
	public:
		virtual any visitBlockStmt(BlockStmt* blockStmt) = 0;
		virtual any visitExpressionStmt(ExpressionStmt* expressionStmt) = 0;
		virtual any visitFunctionStmt(FunctionStmt* functionStmt) = 0;
		virtual any visitIfStmt(IfStmt* ifStmt) = 0;
		virtual any visitWhileStmt(WhileStmt* whileStmt) = 0;
		virtual any visitPrintStmt(PrintStmt* printStmt) = 0;
		virtual any visitReturnStmt(ReturnStmt* returnStmt) = 0;
		virtual any visitVarStmt(VarStmt* varStmt) = 0;
	};

	virtual ~Stmt() { }
	virtual any accept(Visitor* visitor) = 0;
};


class BlockStmt : public Stmt {
public:
	BlockStmt(vector<Stmt*> statements) {
		this->statements = statements;
	}

	~BlockStmt() {
		for (auto _x : statements) {
			 delete _x;
		}
	}

	any accept(Visitor* visitor) {
		return visitor->visitBlockStmt(this);
	}

	vector<Stmt*> statements;
};

class ExpressionStmt : public Stmt {
public:
	ExpressionStmt(Expr* expression) {
		this->expression = expression;
	}

	~ExpressionStmt() {
		if (expression) delete expression;
	}

	any accept(Visitor* visitor) {
		return visitor->visitExpressionStmt(this);
	}

	Expr* expression;
};

class FunctionStmt : public Stmt {
public:
	FunctionStmt(Token* name, vector<Token*> params, vector<Stmt*> body) {
		this->name = name;
		this->params = params;
		this->body = body;
	}

	~FunctionStmt() {
		for (auto _x : body) {
			 delete _x;
		}
	}

	any accept(Visitor* visitor) {
		return visitor->visitFunctionStmt(this);
	}

	Token* name;
	vector<Token*> params;
	vector<Stmt*> body;
};

class IfStmt : public Stmt {
public:
	IfStmt(Expr* condition, Stmt* thenBranch, Stmt* elseBranch) {
		this->condition = condition;
		this->thenBranch = thenBranch;
		this->elseBranch = elseBranch;
	}

	~IfStmt() {
		if (condition) delete condition;
		if (thenBranch) delete thenBranch;
		if (elseBranch) delete elseBranch;
	}

	any accept(Visitor* visitor) {
		return visitor->visitIfStmt(this);
	}

	Expr* condition;
	Stmt* thenBranch;
	Stmt* elseBranch;
};

class WhileStmt : public Stmt {
public:
	WhileStmt(Expr* condition, Stmt* body) {
		this->condition = condition;
		this->body = body;
	}

	~WhileStmt() {
		if (condition) delete condition;
		if (body) delete body;
	}

	any accept(Visitor* visitor) {
		return visitor->visitWhileStmt(this);
	}

	Expr* condition;
	Stmt* body;
};

class PrintStmt : public Stmt {
public:
	PrintStmt(Expr* expression) {
		this->expression = expression;
	}

	~PrintStmt() {
		if (expression) delete expression;
	}

	any accept(Visitor* visitor) {
		return visitor->visitPrintStmt(this);
	}

	Expr* expression;
};

class ReturnStmt : public Stmt {
public:
	ReturnStmt(Token* keyword, Expr* value) {
		this->keyword = keyword;
		this->value = value;
	}

	~ReturnStmt() {
		if (value) delete value;
	}

	any accept(Visitor* visitor) {
		return visitor->visitReturnStmt(this);
	}

	Token* keyword;
	Expr* value;
};

class VarStmt : public Stmt {
public:
	VarStmt(Token* name, Expr* initializer) {
		this->name = name;
		this->initializer = initializer;
	}

	~VarStmt() {
		if (initializer) delete initializer;
	}

	any accept(Visitor* visitor) {
		return visitor->visitVarStmt(this);
	}

	Token* name;
	Expr* initializer;
};
