#pragma once

#include <unordered_map>

#include <token.hpp>
#include <run_time_error.hpp>

using std::unordered_map;

namespace cpplox {

    class Environment {
    public:
        Environment() {
            enclosing = nullptr;
            // std::cout << this << " : " << __func__ << std::endl;
        }

        Environment(Environment* _env) {
            enclosing = _env;
            // std::cout << this << " : " << __func__ << " enclosing: " << _env << std::endl;
        }

        ~Environment();

        void define(string name, any value) {
            // std::cout << this << " : Store => " << name << std::endl;
            values.insert({name, value});
        }

        any get(Token* name) {
            if (values.find(name->lexeme) != values.end()) {
                return values[name->lexeme];
            }
            if (enclosing) {
                return enclosing->get(name);
            }
            // throw "Undefined variable.";
            throw RuntimeError(name, "Undefined variable '" + name->lexeme + "'.");
        }

        void assign(Token* name, any value) {
            if (values.find(name->lexeme) != values.end()) {
                values[name->lexeme] = value;
                return;
            }
            if (enclosing) {
                enclosing->assign(name, value);
                return;
            }
            throw "Undefined variable.";
        }

        Environment* enclosing = nullptr;

    private:
        unordered_map<string, any> values;
    };

}