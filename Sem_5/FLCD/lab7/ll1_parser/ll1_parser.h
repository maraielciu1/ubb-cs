#ifndef LL1_PARSER_H
#define LL1_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SYMBOL_LEN 64
#define MAX_PRODUCTIONS 200
#define MAX_RHS_LEN 20
#define MAX_NON_TERMINALS 50
#define MAX_TERMINALS 100
#define MAX_STACK_SIZE 1000

// Token codes (matching tokens.h)
#define TOKEN_START 256
#define TOKEN_STOP 257
#define TOKEN_IF 258
#define TOKEN_ELSE 259
#define TOKEN_WHILE 260
#define TOKEN_READ 261
#define TOKEN_WRITE 262
#define TOKEN_INT 263
#define TOKEN_FLOAT 264
#define TOKEN_STRING 265
#define TOKEN_CHAR 266
#define TOKEN_IDENTIFIER 267
#define TOKEN_INT_CONST 268
#define TOKEN_FLOAT_CONST 269
#define TOKEN_CHAR_CONST 270
#define TOKEN_STRING_CONST 271
#define TOKEN_ASSIGN 272
#define TOKEN_PLUS 273
#define TOKEN_MINUS 274
#define TOKEN_MUL 275
#define TOKEN_DIV 276
#define TOKEN_MOD 277
#define TOKEN_EQ 278
#define TOKEN_NEQ 279
#define TOKEN_LT 280
#define TOKEN_GT 281
#define TOKEN_LE 282
#define TOKEN_GE 283
#define TOKEN_AND 284
#define TOKEN_OR 285
#define TOKEN_TILDE 286
#define TOKEN_LPAREN 287
#define TOKEN_RPAREN 288
#define TOKEN_LBRACE 289
#define TOKEN_RBRACE 290
#define TOKEN_SEMICOLON 291
#define TOKEN_COMMA 292
#define TOKEN_EOF 0

// Symbol types
typedef enum {
    SYM_TERMINAL,
    SYM_NON_TERMINAL,
    SYM_EPSILON
} SymbolType;

// Symbol structure
typedef struct {
    char name[MAX_SYMBOL_LEN];
    SymbolType type;
    int token_code;  // For terminals
} Symbol;

// Production rule
typedef struct {
    char lhs[MAX_SYMBOL_LEN];  // Left-hand side
    char rhs[MAX_RHS_LEN][MAX_SYMBOL_LEN];  // Right-hand side symbols
    int rhs_len;  // Number of symbols in RHS
    int prod_num;  // Production number
} Production;

// Grammar structure
typedef struct {
    Symbol non_terminals[MAX_NON_TERMINALS];
    Symbol terminals[MAX_TERMINALS];
    Production productions[MAX_PRODUCTIONS];
    int num_non_terminals;
    int num_terminals;
    int num_productions;
    char start_symbol[MAX_SYMBOL_LEN];
} Grammar;

// Parse tree node
typedef struct ParseTreeNode {
    char symbol[MAX_SYMBOL_LEN];
    SymbolType type;
    int token_code;
    int node_id;
    int production_num;  // Production number used to expand this non-terminal (-1 if terminal or not set)
    struct ParseTreeNode* parent;
    struct ParseTreeNode* first_child;
    struct ParseTreeNode* next_sibling;
} ParseTreeNode;

// LL(1) Parser structure
typedef struct {
    Grammar* grammar;
    bool** parsing_table;  // [non_terminal][terminal]
    int* first_sets[MAX_NON_TERMINALS];  // FIRST sets
    int* follow_sets[MAX_NON_TERMINALS];  // FOLLOW sets
    ParseTreeNode* parse_tree;
    int next_node_id;
} LL1Parser;

// Function declarations
Grammar* create_grammar(void);
void add_production(Grammar* g, const char* lhs, const char** rhs, int rhs_len, int prod_num);
int get_non_terminal_index(Grammar* g, const char* name);
int get_terminal_index(Grammar* g, int token_code);
void compute_first_sets(LL1Parser* parser);
void compute_follow_sets(LL1Parser* parser);
void build_parsing_table(LL1Parser* parser);
LL1Parser* create_ll1_parser(Grammar* g);
bool parse(LL1Parser* parser, int* input_tokens, int input_len);
void print_parse_tree_table(LL1Parser* parser, const char* filename);
void print_productions_string(LL1Parser* parser, const char* filename);
void print_derivations_string(LL1Parser* parser, const char* filename);
void free_parser(LL1Parser* parser);
void free_grammar(Grammar* g);

// Helper function for creating parse tree nodes (used by arithmetic grammar)
ParseTreeNode* create_parse_node(LL1Parser* parser, const char* symbol, SymbolType type, int token_code);

#endif

