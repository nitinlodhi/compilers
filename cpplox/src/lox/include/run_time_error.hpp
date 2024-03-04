#pragma once

#include <stdexcept>
#include <string>

#include <token.hpp>

namespace cpplox {
    class RuntimeError : public std::runtime_error {
    public:
        RuntimeError(Token* _token, std::string error)
            : token(_token), std::runtime_error(error) {}

        const Token* token;
    };
}