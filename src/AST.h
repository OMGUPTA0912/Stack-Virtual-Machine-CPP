#pragma once
#include "Token.h"
#include <vector>
#include <memory>
#include <string>

// Abstract Syntax Tree nodes
class ASTVisitor;

class Expr {
public:
    virtual ~Expr() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
};

class Stmt {
public:
    virtual ~Stmt() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
};

// Expressions
class LiteralExpr : public Expr {
public:
    int value; 
    LiteralExpr(int value) : value(value) {}
    void accept(ASTVisitor& visitor) override;
};

class BinaryExpr : public Expr {
public:
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
    
    BinaryExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}
        
    void accept(ASTVisitor& visitor) override;
};

class VariableExpr : public Expr {
public:
    Token name;
    VariableExpr(Token name) : name(std::move(name)) {}
    void accept(ASTVisitor& visitor) override;
};

class AssignExpr : public Expr {
public:
    Token name;
    std::unique_ptr<Expr> value;
    AssignExpr(Token name, std::unique_ptr<Expr> value)
        : name(std::move(name)), value(std::move(value)) {}
    void accept(ASTVisitor& visitor) override;
};

class InputExpr : public Expr {
public:
    InputExpr() = default;
    void accept(ASTVisitor& visitor) override;
};


// Statements
class ExpressionStmt : public Stmt {
public:
    std::unique_ptr<Expr> expression;
    ExpressionStmt(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
    void accept(ASTVisitor& visitor) override;
};

class PrintStmt : public Stmt {
public:
    std::unique_ptr<Expr> expression;
    PrintStmt(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
    void accept(ASTVisitor& visitor) override;
};

// C-like variable declaration e.g. "int x = 5;"
class VarDeclStmt : public Stmt {
public:
    Token name;
    std::unique_ptr<Expr> initializer;
    VarDeclStmt(Token name, std::unique_ptr<Expr> initializer)
        : name(std::move(name)), initializer(std::move(initializer)) {}
    void accept(ASTVisitor& visitor) override;
};

class BlockStmt : public Stmt {
public:
    std::vector<std::unique_ptr<Stmt>> statements;
    BlockStmt(std::vector<std::unique_ptr<Stmt>> statements)
        : statements(std::move(statements)) {}
    void accept(ASTVisitor& visitor) override;
};

class IfStmt : public Stmt {
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;
    
    IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}
        
    void accept(ASTVisitor& visitor) override;
};

class WhileStmt : public Stmt {
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;
    
    WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
        : condition(std::move(condition)), body(std::move(body)) {}
        
    void accept(ASTVisitor& visitor) override;
};

// Visitor Interface
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    virtual void visitLiteralExpr(LiteralExpr& expr) = 0;
    virtual void visitBinaryExpr(BinaryExpr& expr) = 0;
    virtual void visitVariableExpr(VariableExpr& expr) = 0;
    virtual void visitAssignExpr(AssignExpr& expr) = 0;
    virtual void visitInputExpr(InputExpr& expr) = 0;
    
    virtual void visitExpressionStmt(ExpressionStmt& stmt) = 0;
    virtual void visitPrintStmt(PrintStmt& stmt) = 0;
    virtual void visitVarDeclStmt(VarDeclStmt& stmt) = 0;
    virtual void visitBlockStmt(BlockStmt& stmt) = 0;
    virtual void visitIfStmt(IfStmt& stmt) = 0;
    virtual void visitWhileStmt(WhileStmt& stmt) = 0;
};
