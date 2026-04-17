#pragma once
#include "Token.h"
#include <string>
#include <vector>
#include <unordered_map>

class Lexer {
public:
    Lexer(std::string source);
    std::vector<Token> scanTokens();

private:
    std::string source;
    std::vector<Token> tokens;
    
    int start;
    int current;
    int line;
    
    std::unordered_map<std::string, TokenType> keywords;

    bool isAtEnd();
    void scanToken();
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, std::string text);
    bool match(char expected);
    char peek();
    char peekNext();
    
    bool isDigit(char c);
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    
    void identifier();
    void number();
};
