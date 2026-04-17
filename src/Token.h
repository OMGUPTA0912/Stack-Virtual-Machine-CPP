#pragma once
#include <string>

enum class TokenType {
    // Single-character tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // One or two character tokens
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals (C-like integer)
    IDENTIFIER, NUMBER,

    // Keywords (C-like)
    INT, IF, ELSE, WHILE, PRINT, INPUT, 

    TOKEN_EOF, ERROR
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    
    Token(TokenType type, std::string lexeme, int line)
        : type(type), lexeme(std::move(lexeme)), line(line) {}
};
