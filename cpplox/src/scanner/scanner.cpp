#include <scanner.hpp>

namespace cpplox {

    Scanner::Scanner(const string& _source)
        : source(_source) {
        // LOG_FUNCTION_ENTRY();
        // std::cout << "creating scanner object ..." << std::endl;
        line = 1, current = 0, start = 0;
        // LOG_FUNCTION_EXIT();
    }

    bool Scanner::isAtEnd() {
        return current >= source.size();
    }

    void Scanner::addToken(TokenType type) {
        addToken(type, nullptr);
    }

    void Scanner::addToken(TokenType type, any obj) {
        string s = source.substr(start, current - start);
        // std::cout << __func__ << "[" << start << "," << current << "] " << s << std::endl;
        tokens.push_back({type, s, obj, line});
    }

    bool Scanner::match(char expected) {
        if (isAtEnd() || source[current] != expected)
            return false;
        current++;
        return true;
    }

    char Scanner::peek() {
        if (isAtEnd())
            return '\0';
        return source[current];
    }

    char Scanner::peekNext() {
        if (current + 1 >= source.size())
            return '\0';
        return source[current + 1];
    }

    void Scanner::string_() {
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n')
                line++;
            advance();
        }
        if (isAtEnd()) {
            Lox::error(line, "Unterminated string.");
            return;
        }
        advance();
        string s = source.substr(start + 1, current - start - 2);
        addToken(TokenType::STRING, s);
    }

    void Scanner::number() {
        while (isdigit(peek()))
            advance();
        if (peek() == '.' && isdigit(peekNext())) {
            advance();
            while (isdigit(peek()))
                advance();
            string s = source.substr(start, current - start);
            addToken(TokenType::DOUBLE, std::stod(s));
        } else {
            string s = source.substr(start, current - start);
            addToken(TokenType::INTEGER, std::stoi(s));
        }
    }

    void Scanner::identifier() {
        char ch;
        while (isalnum(ch = peek()) || ch == '_')
            advance();
        string lexeme = source.substr(start, current - start);
        auto ref = keywords.find(lexeme);
        if (ref == keywords.end()) {
            addToken(TokenType::IDENTIFIER);
        } else {
            addToken(ref->second);
        }
    }

    char Scanner::advance() {
        return source[current++];
    }

    vector<Token>& Scanner::scanTokens() {
        while (!isAtEnd()) {
            start = current;
            scanToken();
        }

        tokens.push_back({TokenType::EOF_, "", nullptr, line});
        return tokens;
    }

    void Scanner::scanToken() {
        auto ch = advance();

        switch (ch) {
        case '(':
            addToken(TokenType::LEFT_PAREN);
            break;
        case ')':
            addToken(TokenType::RIGHT_PAREN);
            break;
        case '{':
            addToken(TokenType::LEFT_BRACE);
            break;
        case '}':
            addToken(TokenType::RIGHT_BRACE);
            break;
        case ',':
            addToken(TokenType::COMMA);
            break;
        case '.':
            addToken(TokenType::DOT);
            break;
        case '-':
            addToken(TokenType::MINUS);
            break;
        case '+':
            addToken(TokenType::PLUS);
            break;
        case ';':
            addToken(TokenType::SEMICOLON);
            break;
        case '*':
            addToken(TokenType::STAR);
            break;
        case '!':
            addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '>':
            addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        case '<':
            addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '/': {
            if (match('/')) {
                while (peek() != '\n' && !isAtEnd())
                    advance();
            } else {
                addToken(TokenType::SLASH);
            }
        } break;
        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;
        case '\n':
            line++;
            break;
        case '"':
            string_();
            break;
        default:
            if (isdigit(ch)) {
                number();
            } else if (isalpha(ch) || ch == '_') {
                identifier();
            } else {
                Lox::error(line, "Unexpected character !");
            }
            break;
        }
    }

}