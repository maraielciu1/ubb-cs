%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

extern int yylex(void);
extern int yylineno;
extern char* yytext;
extern FILE* yyin;
extern void init_pif_reader(const char* pif_filename);
extern void cleanup_pif_reader(void);

static FILE* productions_file = NULL;
static int production_count = 0;

static void print_production(const char* rule) {
    if (productions_file) {
        fprintf(productions_file, "%d: %s\n", ++production_count, rule);
        printf("%d: %s\n", production_count, rule);
    }
}

void yyerror(const char* s) {
    const char* token = (yytext && yytext[0]) ? yytext : "EOF";
    const char* kind = (yytext && yytext[0]) ? "unexpected token" : "unexpected end of input";
    fprintf(stderr, "[SYNTAX ERROR] %s '%s' at PIF line %d: %s\n", kind, token, yylineno, s);
}
%}

%token START STOP IF THEN ELSE WHILE READ WRITE
%token INT FLOAT STRING CHAR
%token MEAN STDEV TTEST SAMPLE NORMAL BINOMIAL POISSON
%token IDENTIFIER INT_CONST FLOAT_CONST CHAR_CONST STRING_CONST
%token ASSIGN PLUS MINUS MUL DIV MOD
%token EQ NEQ LT GT LE GE AND OR TILDE
%token LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE
%token COMMA DOT SEMICOLON

%left OR
%left AND
%left EQ NEQ LT GT LE GE
%left PLUS MINUS
%left MUL DIV MOD
%right TILDE

%%

program:
    START statement_list STOP {
        print_production("program -> START statement_list STOP");
    }
    ;

statement_list:
    statement {
        print_production("statement_list -> statement");
    }
    | statement_list statement {
        print_production("statement_list -> statement_list statement");
    }
    ;

statement:
    assignment_statement {
        print_production("statement -> assignment_statement");
    }
    | if_statement {
        print_production("statement -> if_statement");
    }
    | while_statement {
        print_production("statement -> while_statement");
    }
    | io_statement {
        print_production("statement -> io_statement");
    }
    | declaration_statement {
        print_production("statement -> declaration_statement");
    }
    | statistical_statement {
        print_production("statement -> statistical_statement");
    }
    ;

assignment_statement:
    IDENTIFIER ASSIGN expression SEMICOLON {
        print_production("assignment_statement -> IDENTIFIER ASSIGN expression SEMICOLON");
    }
    ;

if_statement:
    IF LPAREN condition RPAREN LBRACE statement_list RBRACE {
        print_production("if_statement -> IF LPAREN condition RPAREN LBRACE statement_list RBRACE");
    }
    | IF LPAREN condition RPAREN LBRACE statement_list RBRACE ELSE LBRACE statement_list RBRACE {
        print_production("if_statement -> IF LPAREN condition RPAREN LBRACE statement_list RBRACE ELSE LBRACE statement_list RBRACE");
    }
    ;

while_statement:
    WHILE LPAREN condition RPAREN LBRACE statement_list RBRACE {
        print_production("while_statement -> WHILE LPAREN condition RPAREN LBRACE statement_list RBRACE");
    }
    ;

io_statement:
    WRITE LPAREN expression RPAREN SEMICOLON {
        print_production("io_statement -> WRITE LPAREN expression RPAREN SEMICOLON");
    }
    | READ LPAREN IDENTIFIER RPAREN SEMICOLON {
        print_production("io_statement -> READ LPAREN IDENTIFIER RPAREN SEMICOLON");
    }
    ;

declaration_statement:
    type IDENTIFIER ASSIGN expression SEMICOLON {
        print_production("declaration_statement -> type IDENTIFIER ASSIGN expression SEMICOLON");
    }
    ;

type:
    INT {
        print_production("type -> INT");
    }
    | FLOAT {
        print_production("type -> FLOAT");
    }
    | CHAR {
        print_production("type -> CHAR");
    }
    | STRING {
        print_production("type -> STRING");
    }
    ;

expression:
    term {
        print_production("expression -> term");
    }
    | expression addition_operator term {
        print_production("expression -> expression addition_operator term");
    }
    ;

term:
    factor {
        print_production("term -> factor");
    }
    | term multiplication_operator factor {
        print_production("term -> term multiplication_operator factor");
    }
    ;

factor:
    IDENTIFIER {
        print_production("factor -> IDENTIFIER");
    }
    | constant {
        print_production("factor -> constant");
    }
    | LPAREN expression RPAREN {
        print_production("factor -> LPAREN expression RPAREN");
    }
    | statistical_function_call {
        print_production("factor -> statistical_function_call");
    }
    | sign factor {
        print_production("factor -> sign factor");
    }
    ;

constant:
    INT_CONST {
        print_production("constant -> INT_CONST");
    }
    | FLOAT_CONST {
        print_production("constant -> FLOAT_CONST");
    }
    | CHAR_CONST {
        print_production("constant -> CHAR_CONST");
    }
    | STRING_CONST {
        print_production("constant -> STRING_CONST");
    }
    ;

condition:
    expression relational_operator expression {
        print_production("condition -> expression relational_operator expression");
    }
    | condition logical_operator condition {
        print_production("condition -> condition logical_operator condition");
    }
    | LPAREN condition RPAREN {
        print_production("condition -> LPAREN condition RPAREN");
    }
    ;

addition_operator:
    PLUS {
        print_production("addition_operator -> PLUS");
    }
    | MINUS {
        print_production("addition_operator -> MINUS");
    }
    ;

multiplication_operator:
    MUL {
        print_production("multiplication_operator -> MUL");
    }
    | DIV {
        print_production("multiplication_operator -> DIV");
    }
    | MOD {
        print_production("multiplication_operator -> MOD");
    }
    ;

relational_operator:
    EQ {
        print_production("relational_operator -> EQ");
    }
    | NEQ {
        print_production("relational_operator -> NEQ");
    }
    | LT {
        print_production("relational_operator -> LT");
    }
    | GT {
        print_production("relational_operator -> GT");
    }
    | LE {
        print_production("relational_operator -> LE");
    }
    | GE {
        print_production("relational_operator -> GE");
    }
    ;

logical_operator:
    AND {
        print_production("logical_operator -> AND");
    }
    | OR {
        print_production("logical_operator -> OR");
    }
    ;

sign:
    PLUS {
        print_production("sign -> PLUS");
    }
    | MINUS {
        print_production("sign -> MINUS");
    }
    ;

statistical_statement:
    distribution_statement SEMICOLON {
        print_production("statistical_statement -> distribution_statement SEMICOLON");
    }
    | sampling_statement SEMICOLON {
        print_production("statistical_statement -> sampling_statement SEMICOLON");
    }
    | statistical_function_call SEMICOLON {
        print_production("statistical_statement -> statistical_function_call SEMICOLON");
    }
    ;

distribution_statement:
    IDENTIFIER TILDE distribution_type SEMICOLON {
        print_production("distribution_statement -> IDENTIFIER TILDE distribution_type SEMICOLON");
    }
    | IDENTIFIER TILDE distribution_type LPAREN argument_list RPAREN SEMICOLON {
        print_production("distribution_statement -> IDENTIFIER TILDE distribution_type LPAREN argument_list RPAREN SEMICOLON");
    }
    ;

sampling_statement:
    IDENTIFIER ASSIGN SAMPLE LPAREN distribution_type COMMA expression RPAREN SEMICOLON {
        print_production("sampling_statement -> IDENTIFIER ASSIGN SAMPLE LPAREN distribution_type COMMA expression RPAREN SEMICOLON");
    }
    | IDENTIFIER ASSIGN SAMPLE LPAREN IDENTIFIER COMMA expression RPAREN SEMICOLON {
        print_production("sampling_statement -> IDENTIFIER ASSIGN SAMPLE LPAREN IDENTIFIER COMMA expression RPAREN SEMICOLON");
    }
    ;

statistical_function_call:
    function_name LPAREN argument_list RPAREN {
        print_production("statistical_function_call -> function_name LPAREN argument_list RPAREN");
    }
    ;

function_name:
    MEAN {
        print_production("function_name -> MEAN");
    }
    | STDEV {
        print_production("function_name -> STDEV");
    }
    | TTEST {
        print_production("function_name -> TTEST");
    }
    ;

argument_list:
    expression {
        print_production("argument_list -> expression");
    }
    | expression COMMA argument_list {
        print_production("argument_list -> expression COMMA argument_list");
    }
    ;

distribution_type:
    NORMAL {
        print_production("distribution_type -> NORMAL");
    }
    | BINOMIAL {
        print_production("distribution_type -> BINOMIAL");
    }
    | POISSON {
        print_production("distribution_type -> POISSON");
    }
    ;

%%

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <pif_file>\n", argv[0]);
        fprintf(stderr, "  PIF file format: TOKEN_NAME: position\n");
        return 1;
    }

    // Initialize PIF reader (reads from PIF file instead of source)
    init_pif_reader(argv[1]);

    productions_file = fopen("out/productions.txt", "w");
    if (!productions_file) {
        fprintf(stderr, "Cannot create productions.txt\n");
        cleanup_pif_reader();
        return 1;
    }

    st_init();

    printf("Starting parsing from PIF file: %s\n", argv[1]);
    printf("Grammar: defined in parser.y\n");
    int result = yyparse();
    
    if (result == 0) {
        printf("\nParsing completed successfully!\n");
        printf("Total productions: %d\n", production_count);
        printf("Productions saved to: out/productions.txt\n");
    } else {
        printf("\nParsing failed!\n");
    }

    fclose(productions_file);
    cleanup_pif_reader();

    return result;
}

