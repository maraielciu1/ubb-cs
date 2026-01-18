#ifndef TOKENS_H
#define TOKENS_H

// Token codes start at 256 to avoid clashes with single-byte chars from the lexer
enum Tokens
{
    // Reserved words
    START = 256,
    STOP,
    IF,
    THEN,
    ELSE,
    WHILE,
    READ,
    WRITE,
    INT,
    FLOAT,
    STRING,
    CHAR,
    MEAN,
    STDEV,
    TTEST,
    SAMPLE,
    NORMAL,
    BINOMIAL,
    POISSON,

    // Identifiers & constants
    IDENTIFIER,   // !letter (letter|digit|_)*
    INT_CONST,    // 0 | [+-]?[1-9][0-9]*
    FLOAT_CONST,  // [+-]?[0-9]+\.[0-9]+
    CHAR_CONST,   // 'a' | '7' | '_'
    STRING_CONST, // " [A-Za-z0-9_ .;! ]* "

    // Operators
    ASSIGN, // =
    PLUS,   // +
    MINUS,  // -
    MUL,    // *
    DIV,    // /
    MOD,    // %
    EQ,     // ==
    NEQ,    // !=
    LT,     // <
    GT,     // >
    LE,     // <=
    GE,     // >=
    AND,    // &&
    OR,     // ||
    TILDE,  // ~

    // Separators
    LPAREN,   // (
    RPAREN,   // )
    LBRACK,   // [
    RBRACK,   // ]
    LBRACE,   // {
    RBRACE,   // }
    COMMA,    // ,
    DOT,      // .
    SEMICOLON // ;
};

#endif
