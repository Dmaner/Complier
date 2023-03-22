#ifndef LEXER_H
#define LEXER_H

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/MemoryBuffer.h"

class Lexer;

class Token
{
    friend class Lexer;

public:
    // 定义一个枚举类型TokenKind，表示词法分析器的标记种类
    enum TokenKind : unsigned short
    {
        eoi,     // 表示输入结束
        unknown, // 表示未知的标记
        ident,   // 表示标识符
        number,  // 表示数字
        comma,   // 表示逗号
        colon,   // 表示冒号
        plus,    // 表示加号
        minus,   // 表示减号
        star,    // 表示乘号
        slash,   // 表示除号
        l_paren, // 表示左括号
        r_paren, // 表示右括号
        KW_with  // 表示关键字with
    };

private:
    TokenKind Kind;
    llvm::StringRef Text;

public:
    TokenKind getKind() const { return Kind; }
    llvm::StringRef getText() const { return Text; }

    bool is(TokenKind k) const { return Kind == k; }
    bool isOneOf(TokenKind k1, TokenKind k2) const
    {
        return is(k1) || is(k2);
    }
    template <typename... Ts>
    bool isOneOf(TokenKind k1, TokenKind k2, Ts... ks) const
    {
        return is(k1) || isOneOf(k2, ks...);
    }
};

class Lexer
{
    const char *BufferStart;
    const char *BufferPtr;

public:
    Lexer(const llvm::StringRef &Buffer)
    {
        BufferStart = Buffer.begin();
        BufferPtr = BufferStart;
    }
    void next(Token &token);

private:
    void formToken(Token &Result, const char *TokEnd, Token::TokenKind Kind);
};

#endif