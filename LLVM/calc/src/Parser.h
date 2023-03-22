#ifndef PARSER_H
#define PARSER_H

#include "AST.h"
#include "Lexer.h"
#include "llvm/Support/raw_ostream.h"

class Parser
{
  Lexer &Lex;
  Token Tok;
  bool HasError;

  void error()
  {
    llvm::errs() << "Unexpected: " << Tok.getText() << "\n";
    HasError = true;
  }

  void advance() { Lex.next(Tok); }

  bool expect(Token::TokenKind Kind)
  {
    if (!Tok.is(Kind))
    {
      error();
      return true;
    }
    return false;
  }

  bool consume(Token::TokenKind Kind)
  {
    if (expect(Kind))
      return true;
    advance();
    return false;
  }

  AST *parseCalc();    // 这个函数返回一个指向抽象语法树（AST）的指针，它用于解析一个计算表达式
  Expr *parseExpr();   // 这个函数返回一个指向表达式（Expr）的指针，它用于解析一个加减运算的表达式
  Expr *parseTerm();   // 这个函数返回一个指向表达式（Expr）的指针，它用于解析一个乘除运算的表达式
  Expr *parseFactor(); // 这个函数返回一个指向表达式（Expr）的指针，它用于解析一个括号或数字的因子

public:
  Parser(Lexer &Lex) : Lex(Lex), HasError(false)
  {
    advance();
  }
  AST *parse();
  bool hasError() { return HasError; }
};

#endif