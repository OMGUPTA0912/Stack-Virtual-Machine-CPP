#include "VM.h"
#include <iostream>
#include <stdexcept>

void VM::push(int value) {
    stack.push_back(value);
}

int VM::pop() {
    if (stack.empty()) {
        throw std::runtime_error("Stack underflow.");
    }
    int val = stack.back();
    stack.pop_back();
    return val;
}

uint8_t VM::readByte() {
    return code[ip++];
}

int VM::readConstant() {
    int val = 0;
    val |= (readByte());
    val |= (readByte() << 8);
    val |= (readByte() << 16);
    val |= (readByte() << 24);
    return val;
}

int VM::readShort() {
    int val = 0;
    val |= (readByte());
    val |= (readByte() << 8);
    return val;
}

void VM::interpret(const std::vector<uint8_t>& bytecode) {
    code = bytecode;
    ip = 0;
    stack.clear();
    variables.fill(0);
    
    while (ip < code.size()) {
        Opcode instruction = static_cast<Opcode>(readByte());
        
        switch (instruction) {
            case Opcode::OP_CONST: {
                push(readConstant());
                break;
            }
            case Opcode::OP_LOAD: {
                push(variables[readByte()]);
                break;
            }
            case Opcode::OP_STORE: {
                variables[readByte()] = stack.back(); // leaves value on stack
                break;
            }
            case Opcode::OP_POP: {
                pop();
                break;
            }
            case Opcode::OP_ADD: {
                int b = pop();
                int a = pop();
                push(a + b);
                break;
            }
            case Opcode::OP_SUB: {
                int b = pop();
                int a = pop();
                push(a - b);
                break;
            }
            case Opcode::OP_MUL: {
                int b = pop();
                int a = pop();
                push(a * b);
                break;
            }
            case Opcode::OP_DIV: {
                int b = pop();
                int a = pop();
                push(a / b);
                break;
            }
            case Opcode::OP_EQUAL: {
                int b = pop();
                int a = pop();
                push(a == b ? 1 : 0);
                break;
            }
            case Opcode::OP_LESS: {
                int b = pop();
                int a = pop();
                push(a < b ? 1 : 0);
                break;
            }
            case Opcode::OP_GREATER: {
                int b = pop();
                int a = pop();
                push(a > b ? 1 : 0);
                break;
            }
            case Opcode::OP_NOT: {
                int a = pop();
                push(a == 0 ? 1 : 0);
                break;
            }
            case Opcode::OP_JMP: {
                int offset = readShort();
                ip = offset;
                break;
            }
            case Opcode::OP_JMP_IF_FALSE: {
                int offset = readShort();
                if (stack.back() == 0) { 
                    ip = offset;
                }
                break;
            }
            case Opcode::OP_PRINT: {
                std::cout << pop() << std::endl;
                break;
            }
            case Opcode::OP_INPUT: {
                int val;
                std::cin >> val;
                push(val);
                break;
            }
            case Opcode::OP_HALT: {
                return;
            }
        }
    }
}
