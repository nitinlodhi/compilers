#pragma once

#include "../../interpreter/include/interpreter.hpp"

namespace cpplox
{
    class LoxCallable {
    public:
        virtual int arity() = 0;
        virtual any call(Interpreter* interpreter, vector<any>& arguments) = 0;
    };
}