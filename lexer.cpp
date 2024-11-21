#include <string>
#include <vector>
#include <unordered_map>

enum class TokenType {
    LET, CONST, VAR, TENSOR, MATRIX, VECTOR, NUMBER,
    MODEL, LAYER, TRAIN, TEST, OPTIMIZE,
    PLUS, MINUS, MULTIPLY, DIVIDE,
    LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET,
    SEMICOLON, COLON, COMMA,
    IF, ELSE, FOR, WHILE,
    IDENTIFIER, NUMBER_LITERAL, STRING_LITERAL, EOF_TOKEN, ERROR
};

class Token {
public:
    TokenType type;
    std::string lexeme;
    int line;

    Token(TokenType type, std::string lexeme, int line)
        : type(type), lexeme(lexeme), line(line) {}
};

class Lexer {
public:
    Lexer(const std::string& source) : source(source), start(0), current(0), line(1) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        while (!isAtEnd()) {
            start = current;
            Token token = scanToken();
            if (token.type != TokenType::ERROR) {
                tokens.push_back(token);
            }
        }
        tokens.push_back(Token(TokenType::EOF_TOKEN, "", line));
        return tokens;
    }

private:
    std::string source;
    size_t start;
    size_t current;
    int line;

    bool isAtEnd() {
        return current >= source.length();
    }

    Token scanToken() {
        char c = advance();
        switch (c) {
            case '(': return makeToken(TokenType::LPAREN);
            case ')': return makeToken(TokenType::RPAREN);
            case '{': return makeToken(TokenType::LBRACE);
            case '}': return makeToken(TokenType::RBRACE);
            case '[': return makeToken(TokenType::LBRACKET);
            case ']': return makeToken(TokenType::RBRACKET);
            case ';': return makeToken(TokenType::SEMICOLON);
            case ':': return makeToken(TokenType::COLON);
            case ',': return makeToken(TokenType::COMMA);
            case '+': return makeToken(TokenType::PLUS);
            case '-': return makeToken(TokenType::MINUS);
            case '*': return makeToken(TokenType::MULTIPLY);
            case '/': return handleSlash();
            case ' ':
            case '\r':
            case '\t':
                // Ignore whitespace
                return scanToken();
            case '\n':
                line++;
                return scanToken();
            case '"': return string();
            default:
                if (isDigit(c)) return number();
                if (isAlpha(c)) return identifier();
                return Token(TokenType::ERROR, std::string(1, c), line);
        }
    }

    char advance() {
        return source[current++];
    }

    Token makeToken(TokenType type) {
        return Token(type, source.substr(start, current - start), line);
    }

    bool match(char expected) {
        if (isAtEnd()) return false;
        if (source[current] != expected) return false;
        current++;
        return true;
    }

    char peek() {
        if (isAtEnd()) return '\0';
        return source[current];
    }

    bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }

    bool isAlpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    Token number() {
        while (isDigit(peek())) advance();
        return makeToken(TokenType::NUMBER_LITERAL);
    }

    Token identifier() {
        while (isAlpha(peek()) || isDigit(peek())) advance();
        std::string text = source.substr(start, current - start);
        TokenType type = identifierType(text);
        return makeToken(type);
    }

    TokenType identifierType(const std::string& text) {
        static std::unordered_map<std::string, TokenType> keywords = {
            {"let", TokenType::LET}, {"const", TokenType::CONST}, {"var", TokenType::VAR},
            {"tensor", TokenType::TENSOR}, {"matrix", TokenType::MATRIX}, {"vector", TokenType::VECTOR},
            {"number", TokenType::NUMBER}, {"model", TokenType::MODEL}, {"layer", TokenType::LAYER},
            {"train", TokenType::TRAIN}, {"test", TokenType::TEST}, {"optimize", TokenType::OPTIMIZE},
            {"if", TokenType::IF}, {"else", TokenType::ELSE}, {"for", TokenType::FOR},
            {"while", TokenType::WHILE}
        };

        if (keywords.find(text) != keywords.end()) {
            return keywords[text];
        }
        return TokenType::IDENTIFIER;
    }

    Token string() {
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n') line++;
            advance();
        }

        if (isAtEnd()) {
            return Token(TokenType::ERROR, "Unterminated string.", line);
        }

        // The closing ".
        advance();

        // Trim the surrounding quotes.
        std::string value = source.substr(start + 1, current - start - 2);
        return makeToken(TokenType::STRING_LITERAL);
    }

    Token handleSlash() {
        if (match('/')) {
            // A comment goes until the end of the line.
            while (peek() != '\n' && !isAtEnd()) advance();
            return scanToken();
        } else {
            return makeToken(TokenType::DIVIDE);
        }
    }
};