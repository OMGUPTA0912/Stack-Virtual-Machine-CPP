#include "Compiler.h"
#include <stdexcept>

std::vector<uint8_t> Compiler::compile(const std::vector<std::unique_ptr<Stmt>>& statements) {
    bytecode.clear();
    for (const auto& stmt : statements) {
        if (stmt) {
            stmt->accept(*this);
        }
    }
    emitOpcode(Opcode::OP_HALT);
    return bytecode;
}

void Compiler::emitByte(uint8_t byte) {
    bytecode.push_back(byte);
}

void Compiler::emitOpcode(Opcode op) {
    emitByte(static_cast<uint8_t>(op));
}

void Compiler::emitConstant(int value) {
    emitOpcode(Opcode::OP_CONST);
    // Push 4 bytes for integer
    emitByte(value & 0xFF);
    emitByte((value >> 8) & 0xFF);
    emitByte((value >> 16) & 0xFF);
    emitByte((value >> 24) & 0xFF);
}

int Compiler::emitJump(Opcode op) {
    emitOpcode(op);
    emitByte(0); 
    emitByte(0); 
    return bytecode.size() - 2;
}

void Compiler::patchJump(int offset) {
    int target = bytecode.size();
    if (target > UINT16_MAX) {
        throw std::runtime_error("Program too large.");
    }
    bytecode[offset] = target & 0xFF;
    bytecode[offset + 1] = (target >> 8) & 0xFF;
}

void Compiler::emitLoop(int loopStart) {
    emitOpcode(Opcode::OP_JMP);
    emitByte(loopStart & 0xFF);
    emitByte((loopStart >> 8) & 0xFF);
}

void Compiler::visitLiteralExpr(LiteralExpr& expr) {
    emitConstant(expr.value);
}

void Compiler::visitBinaryExpr(BinaryExpr& expr) {
    expr.left->accept(*this);
    expr.right->accept(*this);
    
    switch (expr.op.type) {
        case TokenType::PLUS: emitOpcode(Opcode::OP_ADD); break;
        case TokenType::MINUS: emitOpcode(Opcode::OP_SUB); break;
        case TokenType::STAR: emitOpcode(Opcode::OP_MUL); break;
        case TokenType::SLASH: emitOpcode(Opcode::OP_DIV); break;
        case TokenType::EQUAL_EQUAL: emitOpcode(Opcode::OP_EQUAL); break;
        case TokenType::BANG_EQUAL:
            emitOpcode(Opcode::OP_EQUAL);
            emitOpcode(Opcode::OP_NOT);
            break;
        case TokenType::LESS: emitOpcode(Opcode::OP_LESS); break;
        case TokenType::LESS_EQUAL:
            emitOpcode(Opcode::OP_GREATER);
            emitOpcode(Opcode::OP_NOT);
            break;
        case TokenType::GREATER: emitOpcode(Opcode::OP_GREATER); break;
        case TokenType::GREATER_EQUAL:
            emitOpcode(Opcode::OP_LESS);
            emitOpcode(Opcode::OP_NOT);
            break;
        default: break;
    }
}

void Compiler::visitVariableExpr(VariableExpr& expr) {
    if (variables.find(expr.name.lexeme) == variables.end()) {
        throw std::runtime_error("Undefined variable: " + expr.name.lexeme);
    }
    emitOpcode(Opcode::OP_LOAD);
    emitByte(variables[expr.name.lexeme]);
}

void Compiler::visitAssignExpr(AssignExpr& expr) {
    expr.value->accept(*this);
    if (variables.find(expr.name.lexeme) == variables.end()) {
        throw std::runtime_error("Assigning to undefined variable: " + expr.name.lexeme);
    }
    emitOpcode(Opcode::OP_STORE);
    emitByte(variables[expr.name.lexeme]);
}

void Compiler::visitInputExpr(InputExpr& expr) {
    emitOpcode(Opcode::OP_INPUT);
}

void Compiler::visitExpressionStmt(ExpressionStmt& stmt) {
    stmt.expression->accept(*this);
    emitOpcode(Opcode::OP_POP); 
}

void Compiler::visitPrintStmt(PrintStmt& stmt) {
    stmt.expression->accept(*this);
    emitOpcode(Opcode::OP_PRINT);
}

void Compiler::visitVarDeclStmt(VarDeclStmt& stmt) {
    if (stmt.initializer) {
        stmt.initializer->accept(*this);
    } else {
        emitConstant(0); 
    }
    
    variables[stmt.name.lexeme] = nextVarIndex++;
    emitOpcode(Opcode::OP_STORE);
    emitByte(variables[stmt.name.lexeme]);
    emitOpcode(Opcode::OP_POP);
}

void Compiler::visitBlockStmt(BlockStmt& stmt) {
    for (const auto& s : stmt.statements) {
        s->accept(*this);
    }
}

void Compiler::visitIfStmt(IfStmt& stmt) {
    stmt.condition->accept(*this);
    
    int thenJump = emitJump(Opcode::OP_JMP_IF_FALSE);
    emitOpcode(Opcode::OP_POP); 
    
    stmt.thenBranch->accept(*this);
    
    int elseJump = emitJump(Opcode::OP_JMP);
    
    patchJump(thenJump);
    emitOpcode(Opcode::OP_POP); 
    
    if (stmt.elseBranch) {
        stmt.elseBranch->accept(*this);
    }
    patchJump(elseJump);
}

void Compiler::visitWhileStmt(WhileStmt& stmt) {
    int loopStart = bytecode.size();
    
    stmt.condition->accept(*this);
    
    int exitJump = emitJump(Opcode::OP_JMP_IF_FALSE);
    emitOpcode(Opcode::OP_POP);
    
    stmt.body->accept(*this);
    emitLoop(loopStart);
    
    patchJump(exitJump);
    emitOpcode(Opcode::OP_POP);
}
