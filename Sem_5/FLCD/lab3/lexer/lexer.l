%option noyywrap yylineno nodefault

%{
#include <stdio.h>
#include <string.h>
#include "tokens.h"
#include "symbol_table.h"

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
%}


ID            \![A-Za-z][A-Za-z0-9_]*
FLOATC        [+\-]?[0-9]+\.[0-9]+
INTC          (0|[+\-]?[1-9][0-9]*)
CHARC         \'([A-Za-z]|[0-9]|_)\' 
STRC          \"([A-Za-z0-9_ \.;!])*\"   
EQOP          "=="
NEQOP         "!="
LEOP          "<="
GEOP          ">="
ANDOP         "&&"
OROP          "\|\|"
WS            [ \t\r]+
NEWLINE       \n

%%

{WS}          { /* skip */ }
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
"FLOAT"    { emit_pif("FLOAT",    -1); }
"STRING"   { emit_pif("STRING",   -1); }
"CHAR"     { emit_pif("CHAR",     -1); }
"MEAN"     { emit_pif("MEAN",     -1); }
"STDEV"    { emit_pif("STDEV",    -1); }
"TTEST"    { emit_pif("TTEST",    -1); }
"SAMPLE"   { emit_pif("SAMPLE",   -1); }
"NORMAL"   { emit_pif("NORMAL",   -1); }
"BINOMIAL" { emit_pif("BINOMIAL", -1); }
"POISSON"  { emit_pif("POISSON",  -1); }

{ID}          { st_insert(yytext); emit_pif("IDENTIFIER",  st_get_index(yytext)); }
{FLOATC}      { st_insert(yytext); emit_pif("FLOAT_CONST", st_get_index(yytext)); }
{INTC}        { st_insert(yytext); emit_pif("INT_CONST",   st_get_index(yytext)); }
{CHARC}       { st_insert(yytext); emit_pif("CHAR_CONST",  st_get_index(yytext)); }
{STRC}        { st_insert(yytext); emit_pif("STRING_CONST",st_get_index(yytext)); }

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

    yylex();

    st_print("out/st.txt");

    fclose(PIF);
    fclose(ERRF);
    fclose(in);

    printf("Lexical analysis complete.\n");
    printf("PIF: out/pif.txt\nST: out/st.txt\nErrors: out/lexical_errors.txt\n");
    return 0;
}
