#pragma once

#include <any>
#include <stdexcept>

namespace cpplox {
    class Return: public std::runtime_error {
    public:
        Return(std::any _val) 
        : std::runtime_error("") {
            value = _val;
        }

        std::any value;
    };
}