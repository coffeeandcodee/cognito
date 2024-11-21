#include <iostream>
#include "lexer.cpp" // Include the lexer header or source file

void printToken(const Token& token) {
    std::cout << "Token(" << static_cast<int>(token.type) << ", \"" << token.lexeme << "\", " << token.line << ")\n";
}

int main() {
    std::string source = R"(
        let x = 10;
        const y = "hello";
        // This is a comment
        if (x > 5) {
            print(y);
        }
    )";
    
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    for (const Token& token : tokens) {
        printToken(token);
    }

    return 0;
}