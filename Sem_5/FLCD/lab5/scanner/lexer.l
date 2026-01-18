%option noyywrap yylineno nodefault

%{
#include <stdio.h>
#include <string.h>
#include "tokens.h"
#include "symbol_table.h"
#include "../fa_rg/fa_rg.h"

static FILE* PIF;
static FILE* ERRF;

static long yycolumn = 1;
#define YY_USER_ACTION yycolumn += yyleng;
static void reset_col(void){ yycolumn = 1; }

static void emit_pif(const char* token, int pos) { fprintf(PIF, "%s: %d\n", token, pos); }
static void lex_error(const char* msg){
  fprintf(ERRF, "Lexical error at line %d, col %ld near '%s': %s\n",
          yylineno, yycolumn, yytext, msg);
}

// FA instances for identifiers and constants
static FiniteAutomata* fa_id = NULL;
static FiniteAutomata* fa_int = NULL;
static FiniteAutomata* fa_float = NULL;
static FiniteAutomata* fa_char = NULL;
static FiniteAutomata* fa_string = NULL;

// Initialize FAs
static void init_fas(void) {
    if (fa_id == NULL) {
        fa_id = create_identifier_fa();
        fa_int = create_int_constant_fa();
        fa_float = create_float_constant_fa();
        fa_char = create_char_constant_fa();
        fa_string = create_string_constant_fa();
    }
}

// Verify token using FA
static int verify_with_fa(const char* text) {
    init_fas();
    
    // Try identifier first
    if (fa_verify(fa_id, text)) {
        return IDENTIFIER;
    }
    // Try float constant
    if (fa_verify(fa_float, text)) {
        return FLOAT_CONST;
    }
    // Try integer constant
    if (fa_verify(fa_int, text)) {
        return INT_CONST;
    }
    // Try character constant
    if (fa_verify(fa_char, text)) {
        return CHAR_CONST;
    }
    // Try string constant
    if (fa_verify(fa_string, text)) {
        return STRING_CONST;
    }
    
    return -1; // Not recognized by any FA
}
%}

EQOP          "=="
NEQOP         "!="
LEOP          "<="
GEOP          ">="
ANDOP         "&&"
OROP          "\|\|"
WS            [ \t\r]+
NEWLINE       \n

POTENTIAL_ID  \![A-Za-z][A-Za-z0-9_]*
POTENTIAL_FLOAT [+\-]?[0-9]+\.[0-9]+
POTENTIAL_INVALID_FLOAT [+\-]?[0-9]+\.[0-9]+\.[0-9.]+
POTENTIAL_INT [+\-]?[0-9]+
POTENTIAL_CHAR \'[A-Za-z0-9_]\'
POTENTIAL_STRING \"[^\"]*\"

%%

{WS}          { }
{NEWLINE}     { reset_col(); }

{EQOP}        { emit_pif("EQ", -1); }
{NEQOP}       { emit_pif("NEQ", -1); }
{LEOP}        { emit_pif("LE", -1); }
{GEOP}        { emit_pif("GE", -1); }
{ANDOP}       { emit_pif("AND", -1); }
{OROP}        { emit_pif("OR", -1);  }

"("           { emit_pif("LPAREN", -1); }
")"           { emit_pif("RPAREN", -1); }
"["           { emit_pif("LBRACK", -1); }
"]"           { emit_pif("RBRACK", -1); }
"{"           { emit_pif("LBRACE", -1); }
"}"           { emit_pif("RBRACE", -1); }
";"           { emit_pif("SEMICOLON", -1); }
","           { emit_pif("COMMA", -1); }
"."           { emit_pif("DOT", -1); }
"+"           { emit_pif("PLUS", -1); }
"-"           { emit_pif("MINUS", -1); }
"*"           { emit_pif("MUL", -1); }
"/"           { emit_pif("DIV", -1); }
"%"           { emit_pif("MOD", -1); }
"="           { emit_pif("ASSIGN", -1); }
"<"           { emit_pif("LT", -1); }
">"           { emit_pif("GT", -1); }
"~"           { emit_pif("TILDE", -1); }

"START"    { emit_pif("START",    -1); }
"STOP"     { emit_pif("STOP",     -1); }
"IF"       { emit_pif("IF",       -1); }
"THEN"     { emit_pif("THEN",     -1); }
"ELSE"     { emit_pif("ELSE",     -1); }
"WHILE"    { emit_pif("WHILE",    -1); }
"READ"     { emit_pif("READ",     -1); }
"WRITE"    { emit_pif("WRITE",    -1); }
"INT"      { emit_pif("INT",      -1); }
"FLOAT"   { emit_pif("FLOAT",     -1); }
"STRING"   { emit_pif("STRING",   -1); }
"CHAR"     { emit_pif("CHAR",     -1); }
"MEAN"     { emit_pif("MEAN",     -1); }
"STDEV"    { emit_pif("STDEV",    -1); }
"TTEST"    { emit_pif("TTEST",    -1); }
"SAMPLE"   { emit_pif("SAMPLE",   -1); }
"NORMAL"   { emit_pif("NORMAL",   -1); }
"BINOMIAL" { emit_pif("BINOMIAL", -1); }
"POISSON"  { emit_pif("POISSON",  -1); }

{POTENTIAL_ID} {
    int token_type = verify_with_fa(yytext);
    if (token_type == IDENTIFIER) {
        st_insert(yytext);
        emit_pif("IDENTIFIER", st_get_index(yytext));
    } else {
        lex_error("Invalid identifier format");
    }
}

{POTENTIAL_INVALID_FLOAT} {
    lex_error("Invalid float constant: multiple decimal points");
}

{POTENTIAL_FLOAT} {
    int token_type = verify_with_fa(yytext);
    if (token_type == FLOAT_CONST) {
        st_insert(yytext);
        emit_pif("FLOAT_CONST", st_get_index(yytext));
    } else {
        lex_error("Invalid float constant format");
    }
}

{POTENTIAL_INT} {
    int token_type = verify_with_fa(yytext);
    if (token_type == INT_CONST) {
        st_insert(yytext);
        emit_pif("INT_CONST", st_get_index(yytext));
    } else {
        lex_error("Invalid integer constant format");
    }
}

{POTENTIAL_CHAR} {
    int token_type = verify_with_fa(yytext);
    if (token_type == CHAR_CONST) {
        st_insert(yytext);
        emit_pif("CHAR_CONST", st_get_index(yytext));
    } else {
        lex_error("Invalid character constant format");
    }
}

{POTENTIAL_STRING} {
    int token_type = verify_with_fa(yytext);
    if (token_type == STRING_CONST) {
        st_insert(yytext);
        emit_pif("STRING_CONST", st_get_index(yytext));
    } else {
        lex_error("Invalid string constant format");
    }
}

.             { lex_error("Unrecognized token"); }

%%

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source>\n", argv[0]);
        return 1;
    }

    FILE* in = fopen(argv[1], "r");
    if (!in) { fprintf(stderr, "Cannot open %s\n", argv[1]); return 1; }
    yyin = in;

    PIF  = fopen("out/pif.txt", "w");
    ERRF = fopen("out/lexical_errors.txt", "w");

    st_init();
    reset_col();
    
    // Initialize FAs
    init_fas();

    yylex();
    
    // Cleanup FAs
    if (fa_id) fa_free(fa_id);
    if (fa_int) fa_free(fa_int);
    if (fa_float) fa_free(fa_float);
    if (fa_char) fa_free(fa_char);
    if (fa_string) fa_free(fa_string);

    st_print("out/st.txt");

    fclose(PIF);
    fclose(ERRF);
    fclose(in);

    printf("Lexical analysis complete.\n");
    printf("PIF: out/pif.txt\nST: out/st.txt\nErrors: out/lexical_errors.txt\n");
    return 0;
}
