#ifndef AST_H
#define AST_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"

class AST;      // 这个类表示一个抽象语法树（AST），它是一个用于表示程序结构的树形数据结构
class Expr;     // 这个类表示一个表达式（Expr），它是一个用于表示计算操作的AST节点
class Factor;   // 这个类表示一个因子（Factor），它是一个用于表示括号或数字的Expr节点
class BinaryOp; // 这个类表示一个二元运算符（BinaryOp），它是一个用于表示加减乘除等运算的Expr节点
class WithDecl; // 这个类表示一个带有声明的表达式（WithDecl），它是一个用于表示变量定义和赋值的Expr节点

class ASTVisitor
{
public:
    virtual void visit(AST &){};
    virtual void visit(Expr &){};
    virtual void visit(Factor &) = 0;
    virtual void visit(BinaryOp &) = 0;
    virtual void visit(WithDecl &) = 0;
};

class AST
{
public:
    virtual ~AST() {}
    virtual void accept(ASTVisitor &V) = 0;
};

class Expr : public AST
{
public:
    Expr() {}
};

class Factor : public Expr
{
public:
    enum ValueKind
    {
        Ident,
        Number
    };

private:
    ValueKind Kind;
    llvm::StringRef Val;

public:
    Factor(ValueKind Kind, llvm::StringRef Val)
        : Kind(Kind), Val(Val) {}
    ValueKind getKind() { return Kind; }
    llvm::StringRef getVal() { return Val; }
    virtual void accept(ASTVisitor &V) override
    {
        V.visit(*this);
    }
};

class BinaryOp : public Expr
{
public:
    enum Operator
    {
        Plus,
        Minus,
        Mul,
        Div
    };

private:
    Expr *Left;
    Expr *Right;
    Operator Op;

public:
    BinaryOp(Operator Op, Expr *L, Expr *R)
        : Op(Op), Left(L), Right(R) {}
    Expr *getLeft() { return Left; }
    Expr *getRight() { return Right; }
    Operator getOperator() { return Op; }
    virtual void accept(ASTVisitor &V) override
    {
        V.visit(*this);
    }
};

class WithDecl : public AST
{
    using VarVector = llvm::SmallVector<llvm::StringRef, 8>;
    VarVector Vars;
    Expr *E;

public:
    WithDecl(llvm::SmallVector<llvm::StringRef, 8> Vars,
             Expr *E)
        : Vars(Vars), E(E) {}
    VarVector::const_iterator begin() { return Vars.begin(); }
    VarVector::const_iterator end() { return Vars.end(); }
    Expr *getExpr() { return E; }
    virtual void accept(ASTVisitor &V) override
    {
        V.visit(*this);
    }
};
#endif