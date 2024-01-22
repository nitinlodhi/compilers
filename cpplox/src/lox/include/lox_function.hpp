#pragma once

#include "lox_callable.hpp"
#include <return.hpp>

namespace cpplox {
    class LoxFunction : public LoxCallable {
    public:
        LoxFunction(FunctionStmt* _declaration) {
            declaration = _declaration;
        }

        int arity() {
            return declaration->params.size();
        }

        any call(Interpreter* interpreter, vector<any>& arguments) {
            Environment* newEnv = new Environment();
            for (int i = 0; i < declaration->params.size(); i++) {
                newEnv->define(declaration->params[i]->lexeme, arguments[i]);
            }
            try {
                interpreter->executeBlock(declaration->body->statements, newEnv);
            } catch (Return& result) {
                return result.value;
            }
            return nullptr;
        }

        FunctionStmt* declaration;
    };
}