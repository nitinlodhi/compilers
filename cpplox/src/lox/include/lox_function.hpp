#pragma once

#include <return.hpp>
#include "lox_callable.hpp"

namespace cpplox {
    class LoxFunction : public LoxCallable {
    public:
        LoxFunction(FunctionStmt* _declaration, Environment* _env) {
            declaration = _declaration;
            closure = *_env;
            // std::cout << __func__ << " " << arity() << " => " << this << std::endl;
            // std::cout << __func__ << " => " << this << " : " << declaration->name->lexeme << std::endl;
        }

        int arity() {
            return declaration->params.size();
        }

        ~LoxFunction() {
            // std::cout << __func__ << " => " << this << std::endl;
            // delete declaration;
        }

        any call(Interpreter* interpreter, vector<any>& arguments) {
            // std::cout << __func__ << std::endl;
            Return result(nullptr);
            auto temp = interpreter->environment;
            unique_ptr<Environment> env{new Environment(&closure)};

            for (int i = 0; i < declaration->params.size(); i++) {
                env->define(declaration->params[i]->lexeme, arguments[i]);
            }
            try {
                interpreter->executeBlock(declaration->body, std::move(env));
            } catch (Return& r) {
                result = r;
            }

            interpreter->environment = temp;
            return result.value;
        }

        FunctionStmt* declaration;
        Environment closure;
    };
}