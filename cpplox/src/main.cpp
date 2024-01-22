#include <iostream>
#include <lox.hpp>

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cerr << "Usage cpplox [script]" << std::endl;
        exit(64);
    } else if (argc == 2) {
        cpplox::Lox::runFile(argv[1]);
    } else {
        cpplox::Lox::runPrompt();
    }
    return 0;
}