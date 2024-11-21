#ifndef AST_H
#define AST_H

#include <memory>
#include <string>

// Base class for all AST nodes
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(class ASTVisitor &visitor) = 0;
};

// Expression base class
class Expr : public ASTNode {};

// Binary expression node
class BinaryExpr : public Expr {
public:
    std::unique_ptr<Expr> left;
    std::string op;
    std::unique_ptr<Expr> right;

    BinaryExpr(std::unique_ptr<Expr> left, std::string op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}

    void accept(ASTVisitor &visitor) override;
};

// Literal expression node
class LiteralExpr : public Expr {
public:
    std::string value;

    LiteralExpr(std::string value) : value(value) {}

    void accept(ASTVisitor &visitor) override;
};

#endif // AST_H