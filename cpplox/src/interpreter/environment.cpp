#include <environment.hpp>
#include <lox_function.hpp>

namespace cpplox {
    Environment::~Environment() {
        // if (enclosing) {
        //     delete enclosing;
        // }
        // for (auto& [_, value] : values) {
        //     if (value.type() == typeid(LoxFunction*))
        //         delete any_cast<LoxFunction*>(value);
        // }
    }
}