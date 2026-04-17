#pragma once
#include "AST.h"
#include "Opcode.h"
#include <vector>
#include <unordered_map>
#include <string>

class Compiler : public ASTVisitor {
public:
    std::vector<uint8_t> compile(const std::vector<std::unique_ptr<Stmt>>& statements);

    void visitLiteralExpr(LiteralExpr& expr) override;
    void visitBinaryExpr(BinaryExpr& expr) override;
    void visitVariableExpr(VariableExpr& expr) override;
    void visitAssignExpr(AssignExpr& expr) override;
    void visitInputExpr(InputExpr& expr) override;
    
    void visitExpressionStmt(ExpressionStmt& stmt) override;
    void visitPrintStmt(PrintStmt& stmt) override;
    void visitVarDeclStmt(VarDeclStmt& stmt) override;
    void visitBlockStmt(BlockStmt& stmt) override;
    void visitIfStmt(IfStmt& stmt) override;
    void visitWhileStmt(WhileStmt& stmt) override;

private:
    std::vector<uint8_t> bytecode;
    std::unordered_map<std::string, uint8_t> variables; 
    uint8_t nextVarIndex = 0;

    void emitByte(uint8_t byte);
    void emitOpcode(Opcode op);
    void emitConstant(int value);
    
    int emitJump(Opcode op);
    void patchJump(int offset);
    void emitLoop(int loopStart);
};
