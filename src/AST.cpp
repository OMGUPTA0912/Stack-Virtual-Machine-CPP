#include "AST.h"

void LiteralExpr::accept(ASTVisitor& visitor) { visitor.visitLiteralExpr(*this); }
void BinaryExpr::accept(ASTVisitor& visitor) { visitor.visitBinaryExpr(*this); }
void VariableExpr::accept(ASTVisitor& visitor) { visitor.visitVariableExpr(*this); }
void AssignExpr::accept(ASTVisitor& visitor) { visitor.visitAssignExpr(*this); }
void InputExpr::accept(ASTVisitor& visitor) { visitor.visitInputExpr(*this); }

void ExpressionStmt::accept(ASTVisitor& visitor) { visitor.visitExpressionStmt(*this); }
void PrintStmt::accept(ASTVisitor& visitor) { visitor.visitPrintStmt(*this); }
void VarDeclStmt::accept(ASTVisitor& visitor) { visitor.visitVarDeclStmt(*this); }
void BlockStmt::accept(ASTVisitor& visitor) { visitor.visitBlockStmt(*this); }
void IfStmt::accept(ASTVisitor& visitor) { visitor.visitIfStmt(*this); }
void WhileStmt::accept(ASTVisitor& visitor) { visitor.visitWhileStmt(*this); }
