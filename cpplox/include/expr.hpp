#pragma once

#include <token.hpp>

class AssignExpr;
class BinaryExpr;
class CallExpr;
class GroupingExpr;
class LiteralExpr;
class UnaryExpr;
class VariableExpr;

class Expr {
public:
	class Visitor {
	public:
		virtual any visitAssignExpr(AssignExpr* assignExpr) = 0;
		virtual any visitBinaryExpr(BinaryExpr* binaryExpr) = 0;
		virtual any visitCallExpr(CallExpr* callExpr) = 0;
		virtual any visitGroupingExpr(GroupingExpr* groupingExpr) = 0;
		virtual any visitLiteralExpr(LiteralExpr* literalExpr) = 0;
		virtual any visitUnaryExpr(UnaryExpr* unaryExpr) = 0;
		virtual any visitVariableExpr(VariableExpr* variableExpr) = 0;
	};

	virtual ~Expr() { }
	virtual any accept(Visitor* visitor) = 0;
};


class AssignExpr : public Expr {
public:
	AssignExpr(Token* name, Expr* value) {
		this->name = name;
		this->value = value;
	}

	~AssignExpr() {
		if (value) delete value;
	}

	any accept(Visitor* visitor) {
		return visitor->visitAssignExpr(this);
	}

	Token* name;
	Expr* value;
};

class BinaryExpr : public Expr {
public:
	BinaryExpr(Expr* left, Token* op, Expr* right) {
		this->left = left;
		this->op = op;
		this->right = right;
	}

	~BinaryExpr() {
		if (left) delete left;
		if (right) delete right;
	}

	any accept(Visitor* visitor) {
		return visitor->visitBinaryExpr(this);
	}

	Expr* left;
	Token* op;
	Expr* right;
};

class CallExpr : public Expr {
public:
	CallExpr(Expr* callee, Token* paren, vector<Expr*> arguments) {
		this->callee = callee;
		this->paren = paren;
		this->arguments = arguments;
	}

	~CallExpr() {
		if (callee) delete callee;
		for (auto _x : arguments) {
			 delete _x;
		}
	}

	any accept(Visitor* visitor) {
		return visitor->visitCallExpr(this);
	}

	Expr* callee;
	Token* paren;
	vector<Expr*> arguments;
};

class GroupingExpr : public Expr {
public:
	GroupingExpr(Expr* expr) {
		this->expr = expr;
	}

	~GroupingExpr() {
		if (expr) delete expr;
	}

	any accept(Visitor* visitor) {
		return visitor->visitGroupingExpr(this);
	}

	Expr* expr;
};

class LiteralExpr : public Expr {
public:
	LiteralExpr(any value) {
		this->value = value;
	}

	~LiteralExpr() {
	}

	any accept(Visitor* visitor) {
		return visitor->visitLiteralExpr(this);
	}

	any value;
};

class UnaryExpr : public Expr {
public:
	UnaryExpr(Token* op, Expr* right) {
		this->op = op;
		this->right = right;
	}

	~UnaryExpr() {
		if (right) delete right;
	}

	any accept(Visitor* visitor) {
		return visitor->visitUnaryExpr(this);
	}

	Token* op;
	Expr* right;
};

class VariableExpr : public Expr {
public:
	VariableExpr(Token* name) {
		this->name = name;
	}

	~VariableExpr() {
	}

	any accept(Visitor* visitor) {
		return visitor->visitVariableExpr(this);
	}

	Token* name;
};
