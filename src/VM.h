#pragma once
#include "Opcode.h"
#include <vector>
#include <array>
#include <string>

class VM {
public:
    void interpret(const std::vector<uint8_t>& bytecode);

private:
    std::vector<uint8_t> code;
    int ip = 0; 
    
    std::vector<int> stack;
    std::array<int, 256> variables;

    void push(int value);
    int pop();
    
    uint8_t readByte();
    int readConstant();
    int readShort();
};
