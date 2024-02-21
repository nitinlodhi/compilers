#include <environment.hpp>
#include <lox_function.hpp>
#include <cassert>

namespace cpplox {
    Environment::~Environment() {
        // std::cout << this << " : " << __func__ << std::endl;
        // std::cout << "Num Keys => " << values.size() << std::endl;
        for (auto& [key, value] : values) {
            // std::cout << "Key => " << key << std::endl;
            if (value.type() == typeid(LoxFunction*)) {
                delete any_cast<LoxFunction*>(value);
            }
        }
    }
}