#pragma once

#include <return.hpp>
#include "lox_callable.hpp"

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
            Return result(nullptr);
            Environment* env = new Environment(interpreter->getEnv());

            for (int i = 0; i < declaration->params.size(); i++) {
                env->define(declaration->params[i]->lexeme, arguments[i]);
            }
            try {
                interpreter->executeBlock(declaration->body->statements, env);
            } catch (Return& r) {
                result = r;
            }
            return result.value;
        }

        FunctionStmt* declaration;
    };
}