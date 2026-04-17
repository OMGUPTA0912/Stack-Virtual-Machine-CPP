#pragma once
#include <cstdint>

// Instruction Set Architecture (ISA) for our Stack-Based VM
enum class Opcode : uint8_t {
    // Stack manipulation
    OP_CONST,    // Push constant value to stack
    OP_LOAD,     // Load variable value to stack
    OP_STORE,    // Store top of stack to variable/memory
    OP_POP,      // Pop top of stack
    
    // Arithmetic & Logic
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_EQUAL,
    OP_LESS,
    OP_GREATER,
    OP_NOT,

    // Control Flow
    OP_JMP,      // Jump unconditional
    OP_JMP_IF_FALSE, // Jump if top of stack is false (0)
    
    // IO & System
    OP_PRINT,    // Print top of stack
    OP_INPUT,    // Request user input to stack
    OP_HALT      // Halt VM
};
