%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "symbol_table.h"

extern int yylex(void);
extern int yylineno;
extern char* yytext;
extern void init_pif_reader(const char* pif_filename);
extern void cleanup_pif_reader(void);

FILE* c_out_file = NULL;
static FILE* productions_file = NULL;
static int production_count = 0;

/* --- Debugging / Logging Function --- */
static void print_production(const char* rule) {
    if (productions_file) {
        fprintf(productions_file, "%d: %s\n", ++production_count, rule);
    }
    // We comment out the console print to keep the output clean for the user
    // printf("%d: %s\n", production_count, rule); 
}

void yyerror(const char* s) {
    fprintf(stderr, "[SYNTAX ERROR] at line %d: %s\n", yylineno, s);
}

/* --- String Concatenation Helper (for Code Gen) --- */
char* cat(int count, ...) {
    va_list ap;
    int len = 0;
    
    // Pass 1: Measure length
    va_start(ap, count);
    for(int i = 0; i < count; i++) {
        char* s = va_arg(ap, char*);
        if(s) len += strlen(s);
    }
    va_end(ap);

    char* res = (char*)malloc(len + 1);
    if (!res) exit(1);
    res[0] = '\0';

    // Pass 2: Concatenate and free constituents
    va_start(ap, count);
    for(int i = 0; i < count; i++) {
        char* s = va_arg(ap, char*);
        if(s) {
            strcat(res, s);
            free(s); 
        }
    }
    va_end(ap);
    return res;
}

char* new_str(const char* s) {
    return s ? strdup(s) : strdup("");
}
%}

%union {
    char* code;
}

%token <code> IDENTIFIER INT_CONST FLOAT_CONST CHAR_CONST STRING_CONST
%type <code> program statement_list statement assignment_statement if_statement while_statement
%type <code> io_statement declaration_statement type expression term factor constant
%type <code> condition addition_operator multiplication_operator relational_operator logical_operator
%type <code> statistical_statement distribution_statement sampling_statement statistical_function_call
%type <code> function_name argument_list distribution_type sign

%token START STOP IF THEN ELSE WHILE READ WRITE
%token INT FLOAT STRING CHAR
%token MEAN STDEV TTEST SAMPLE NORMAL BINOMIAL POISSON
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

        /* --- WRITE THE HEADER AND RUNTIME LIBRARY --- */
        fprintf(c_out_file, "#include <stdio.h>\n");
        fprintf(c_out_file, "#include <stdlib.h>\n");
        fprintf(c_out_file, "#include <math.h>\n");
        fprintf(c_out_file, "#include <time.h>\n\n");
        
        // Embedded C Runtime for Statistics
        fprintf(c_out_file, "/* --- MINI-LANG RUNTIME START --- */\n");
        fprintf(c_out_file, "double calc_mean(double* arr, int n) {\n");
        fprintf(c_out_file, "    double sum = 0; for(int i=0; i<n; i++) sum+=arr[i]; return sum/n;\n");
        fprintf(c_out_file, "}\n");
        
        fprintf(c_out_file, "double calc_stdev(double* arr, int n) {\n");
        fprintf(c_out_file, "    double m = calc_mean(arr, n); double sum = 0;\n");
        fprintf(c_out_file, "    for(int i=0; i<n; i++) sum += pow(arr[i]-m, 2);\n");
        fprintf(c_out_file, "    return sqrt(sum/n);\n");
        fprintf(c_out_file, "}\n");

        fprintf(c_out_file, "double sample_normal(double param) {\n");
        fprintf(c_out_file, "    // Box-Muller transform\n");
        fprintf(c_out_file, "    double u = (double)rand() / RAND_MAX; double v = (double)rand() / RAND_MAX;\n");
        fprintf(c_out_file, "    return sqrt(-2.0 * log(u)) * cos(2.0 * 3.14159 * v) * param;\n");
        fprintf(c_out_file, "}\n");
        fprintf(c_out_file, "/* --- MINI-LANG RUNTIME END --- */\n\n");

        fprintf(c_out_file, "int main() {\n");
        fprintf(c_out_file, "    srand(time(NULL)); // Seed RNG\n");
        fprintf(c_out_file, "%s", $2); // The generated code body
        fprintf(c_out_file, "    return 0;\n");
        fprintf(c_out_file, "}\n");
        
        free($2);
    }
    ;

statement_list:
    statement {
        print_production("statement_list -> statement");
        $$ = $1;
    }
    | statement_list statement {
        print_production("statement_list -> statement_list statement");
        $$ = cat(2, $1, $2);
    }
    ;

statement:
    assignment_statement    { print_production("statement -> assignment"); $$ = $1; }
    | if_statement          { print_production("statement -> if"); $$ = $1; }
    | while_statement       { print_production("statement -> while"); $$ = $1; }
    | io_statement          { print_production("statement -> io"); $$ = $1; }
    | declaration_statement { print_production("statement -> decl"); $$ = $1; }
    | statistical_statement { print_production("statement -> stat"); $$ = $1; }
    ;

assignment_statement:
    IDENTIFIER ASSIGN expression SEMICOLON {
        print_production("assignment -> ID = expr");
        $$ = cat(4, new_str($1), new_str(" = "), $3, new_str(";\n"));
    }
    ;

if_statement:
    IF LPAREN condition RPAREN LBRACE statement_list RBRACE {
        print_production("if_statement -> IF...");
        $$ = cat(5, new_str("if ("), $3, new_str(") {\n"), $6, new_str("}\n"));
    }
    | IF LPAREN condition RPAREN LBRACE statement_list RBRACE ELSE LBRACE statement_list RBRACE {
        print_production("if_statement -> IF...ELSE...");
        $$ = cat(7, new_str("if ("), $3, new_str(") {\n"), $6, new_str("} else {\n"), $10, new_str("}\n"));
    }
    ;

while_statement:
    WHILE LPAREN condition RPAREN LBRACE statement_list RBRACE {
        print_production("while_statement");
        $$ = cat(5, new_str("while ("), $3, new_str(") {\n"), $6, new_str("}\n"));
    }
    ;

io_statement:
    WRITE LPAREN expression RPAREN SEMICOLON {
        print_production("io -> WRITE");
        // Using %g for generic double printing
        $$ = cat(3, new_str("printf(\"Output: %g\\n\", (double)"), $3, new_str(");\n"));
    }
    | READ LPAREN IDENTIFIER RPAREN SEMICOLON {
        print_production("io -> READ");
        $$ = cat(5, new_str("printf(\"Enter value for "), new_str($3), new_str(": \"); scanf(\"%lf\", &"), new_str($3), new_str(");\n"));
    }
    ;

declaration_statement:
    type IDENTIFIER ASSIGN expression SEMICOLON {
        print_production("decl -> type ID = expr");
        $$ = cat(6, $1, new_str(" "), new_str($2), new_str(" = "), $4, new_str(";\n"));
    }
    ;

type:
    INT      { print_production("type -> INT"); $$ = new_str("int"); }
    | FLOAT  { print_production("type -> FLOAT"); $$ = new_str("double"); }
    | CHAR   { print_production("type -> CHAR"); $$ = new_str("char"); }
    | STRING { print_production("type -> STRING"); $$ = new_str("char*"); }
    ;

expression:
    term { print_production("expr -> term"); $$ = $1; }
    | expression addition_operator term {
        print_production("expr -> expr + term");
        $$ = cat(3, $1, $2, $3);
    }
    ;

term:
    factor { print_production("term -> factor"); $$ = $1; }
    | term multiplication_operator factor {
        print_production("term -> term * factor");
        $$ = cat(3, $1, $2, $3);
    }
    ;

factor:
    IDENTIFIER { print_production("factor -> ID"); $$ = new_str($1); }
    | constant { print_production("factor -> const"); $$ = $1; }
    | LPAREN expression RPAREN { 
        print_production("factor -> (expr)");
        $$ = cat(3, new_str("("), $2, new_str(")")); 
    }
    | statistical_function_call { print_production("factor -> stat_call"); $$ = $1; }
    | sign factor { print_production("factor -> sign factor"); $$ = cat(2, $1, $2); }
    ;

constant:
    INT_CONST     { $$ = new_str($1); }
    | FLOAT_CONST { $$ = new_str($1); }
    | CHAR_CONST  { $$ = new_str($1); }
    | STRING_CONST { $$ = new_str($1); }
    ;

condition:
    expression relational_operator expression {
        print_production("cond -> expr op expr");
        $$ = cat(3, $1, $2, $3);
    }
    | condition logical_operator condition {
        print_production("cond -> cond op cond");
        $$ = cat(3, $1, $2, $3);
    }
    | LPAREN condition RPAREN {
        $$ = cat(3, new_str("("), $2, new_str(")"));
    }
    ;

addition_operator:
    PLUS  { $$ = new_str(" + "); } | MINUS { $$ = new_str(" - "); } ;

multiplication_operator:
    MUL { $$ = new_str(" * "); } | DIV { $$ = new_str(" / "); } | MOD { $$ = new_str(" % "); } ;

relational_operator:
    EQ { $$ = new_str(" == "); } | NEQ { $$ = new_str(" != "); } | LT { $$ = new_str(" < "); }
    | GT { $$ = new_str(" > "); } | LE { $$ = new_str(" <= "); } | GE { $$ = new_str(" >= "); } ;

logical_operator:
    AND { $$ = new_str(" && "); } | OR { $$ = new_str(" || "); } ;

sign:
    PLUS { $$ = new_str("+"); } | MINUS { $$ = new_str("-"); } ;

/* --- STATISTICAL TRANSLATION --- */

statistical_statement:
    distribution_statement { print_production("stat -> dist"); $$ = $1; }
    | sampling_statement { print_production("stat -> sample"); $$ = $1; }
    | statistical_function_call SEMICOLON { 
        print_production("stat -> func_call"); 
        $$ = cat(2, $1, new_str(";\n")); 
    }
    ;

distribution_statement:
    IDENTIFIER TILDE distribution_type SEMICOLON {
        /* In C, we just print a log for metadata configuration */
        print_production("dist -> ID ~ type");
        $$ = cat(4, new_str("printf(\"Configured distribution for "), new_str($1), new_str("\\n\");\n"), new_str("/* Metadata only */\n"));
    }
    | IDENTIFIER TILDE distribution_type LPAREN argument_list RPAREN SEMICOLON {
        print_production("dist -> ID ~ type(args)");
        $$ = new_str("/* Parametrized distribution setup (ignored in simple C gen) */\n");
    }
    ;

sampling_statement:
    IDENTIFIER ASSIGN SAMPLE LPAREN distribution_type COMMA expression RPAREN SEMICOLON {
        print_production("sample -> direct");
        /* Translates SAMPLE(NORMAL, 5) into sample_normal(5) */
        $$ = cat(6, new_str($1), new_str(" = sample_"), $5, new_str("("), $7, new_str(");\n"));
    }
    | IDENTIFIER ASSIGN SAMPLE LPAREN IDENTIFIER COMMA expression RPAREN SEMICOLON {
        print_production("sample -> indirect");
        /* Fallback for sampling from variable config */
        $$ = cat(4, new_str($1), new_str(" = sample_normal("), $7, new_str(");\n"));
    }
    ;

statistical_function_call:
    function_name LPAREN argument_list RPAREN {
        print_production("stat_call -> func(args)");
        /* C99 TRICK: 
           Translate MEAN(a,b,c) -> calc_mean((double[]){a,b,c}, sizeof((double[]){a,b,c})/sizeof(double))
           This allows us to pass a list of arguments without counting them in the parser.
        */
        char* args = $3;
        // Need to duplicate args since cat() will free it and we use it twice
        char* args_dup = strdup(args);
        $$ = cat(6, $1, new_str("((double[]){"), args, new_str("}, sizeof((double[]){"), new_str(args_dup), new_str("})/sizeof(double))"));
        free(args_dup);
    }
    ;

function_name:
    MEAN    { $$ = new_str("calc_mean"); }
    | STDEV { $$ = new_str("calc_stdev"); }
    | TTEST { $$ = new_str("perform_ttest"); /* Stub */ }
    ;

argument_list:
    expression { $$ = $1; }
    | expression COMMA argument_list {
        $$ = cat(3, $1, new_str(", "), $3);
    }
    ;

distribution_type:
    NORMAL    { $$ = new_str("normal"); }
    | BINOMIAL { $$ = new_str("binomial"); }
    | POISSON  { $$ = new_str("poisson"); }
    ;

%%

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <pif_file>\n", argv[0]);
        return 1;
    }

    init_pif_reader(argv[1]);
    
    c_out_file = fopen("out/generated_program.c", "w");
    productions_file = fopen("out/productions.txt", "w");

    if (!c_out_file || !productions_file) {
        perror("Output file creation failed");
        return 1;
    }

    printf("Starting Translation...\n");
    yyparse();

    fclose(c_out_file);
    fclose(productions_file);
    cleanup_pif_reader();
    
    printf("Done! Compile with: gcc out/generated_program.c -o program -lm\n");
    return 0;
}
