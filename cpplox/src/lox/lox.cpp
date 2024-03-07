#include <fstream>
#include <sstream>

#include <interpreter.hpp>
#include <lox.hpp>
#include <parser.hpp>
#include <scanner.hpp>

namespace cpplox {

    bool Lox::hadError = false;

    void Lox::runFile(const string& filename) {
        std::cout << "reading file " << filename << std::endl;
        std::ifstream in(filename);

        if (in) {
            std::ostringstream ss;
            ss << in.rdbuf();
            run(ss.str());

            if (hadError)
                exit(65);
        }
    }

    void Lox::runPrompt() {
        string line;
        while (true) {
            std::cout << "> ";
            if (std::getline(std::cin, line)) {
                run(line);
                hadError = false;
            } else {
                break;
            }
        }
    }

    void Lox::run(const string& src) {
        // LOG_FUNCTION_ENTRY();

        Scanner scanner(src);
        int i = 0;
        auto& tokens = scanner.scanTokens();
#if 0
    std::cout << "Total tokens: " << tokens.size() << std::endl;
    for (auto &token: tokens) {
        std::cout << "[" << i++ << "] " << token.toString() << std::endl;
        // std::cout << token << std::endl;
    }
#endif
        Parser parser(tokens);
        auto stmts = parser.parse();

        if (hadError)
            exit(65);
        Interpreter(stmts).interpret();
    }

    void Lox::error(int line, string message) {
        report(line, "", message);
    }

    void Lox::error(Token* token, string message) {
        if (token->type == TokenType::EOF_) {
            report(token->line, "at end", message);
        } else {
            report(token->line, "at '" + token->lexeme + "'", message);
        }
    }

    void Lox::report(int line, string where, string message) {
        std::cerr << "[Line " + std::to_string(line) + "] Error " + where +
                         ": " + message
                  << std::endl;
        hadError = true;
    }

}