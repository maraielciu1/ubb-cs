#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbol_table.h"
#include "y.tab.h"

// PIF file reading
static FILE* pif_file = NULL;
static FILE* st_file = NULL;
static char current_line[256];
static int token_pos = -1;
static int pif_line_num = 0;

// Variables expected by parser
int yylineno = 1;
static char current_token_name[64] = "";
char* yytext = current_token_name;

// Semantic value for the parser
extern YYSTYPE yylval;

// Simple index-to-value mapping for symbol table
#define MAX_ST_ENTRIES 1000
static char* st_values[MAX_ST_ENTRIES];
static int st_max_index = -1;

static void init_st_mapping() {
    for (int i = 0; i < MAX_ST_ENTRIES; i++) {
        st_values[i] = NULL;
    }
    st_max_index = -1;
}

static void set_st_value(int index, const char* value) {
    if (index >= 0 && index < MAX_ST_ENTRIES) {
        if (st_values[index]) {
            free(st_values[index]);
        }
        st_values[index] = strdup(value);
        if (index > st_max_index) {
            st_max_index = index;
        }
    }
}

static const char* get_st_value(int index) {
    if (index >= 0 && index < MAX_ST_ENTRIES && st_values[index]) {
        return st_values[index];
    }
    return NULL;
}

// Token name to token code mapping
static int token_name_to_code(const char* token_name) {
    // Reserved words
    if (strcmp(token_name, "START") == 0) return START;
    if (strcmp(token_name, "STOP") == 0) return STOP;
    if (strcmp(token_name, "IF") == 0) return IF;
    if (strcmp(token_name, "THEN") == 0) return THEN;
    if (strcmp(token_name, "ELSE") == 0) return ELSE;
    if (strcmp(token_name, "WHILE") == 0) return WHILE;
    if (strcmp(token_name, "READ") == 0) return READ;
    if (strcmp(token_name, "WRITE") == 0) return WRITE;
    if (strcmp(token_name, "INT") == 0) return INT;
    if (strcmp(token_name, "FLOAT") == 0) return FLOAT;
    if (strcmp(token_name, "STRING") == 0) return STRING;
    if (strcmp(token_name, "CHAR") == 0) return CHAR;
    if (strcmp(token_name, "MEAN") == 0) return MEAN;
    if (strcmp(token_name, "STDEV") == 0) return STDEV;
    if (strcmp(token_name, "TTEST") == 0) return TTEST;
    if (strcmp(token_name, "SAMPLE") == 0) return SAMPLE;
    if (strcmp(token_name, "NORMAL") == 0) return NORMAL;
    if (strcmp(token_name, "BINOMIAL") == 0) return BINOMIAL;
    if (strcmp(token_name, "POISSON") == 0) return POISSON;
    
    // Identifiers and constants
    if (strcmp(token_name, "IDENTIFIER") == 0) return IDENTIFIER;
    if (strcmp(token_name, "INT_CONST") == 0) return INT_CONST;
    if (strcmp(token_name, "FLOAT_CONST") == 0) return FLOAT_CONST;
    if (strcmp(token_name, "CHAR_CONST") == 0) return CHAR_CONST;
    if (strcmp(token_name, "STRING_CONST") == 0) return STRING_CONST;
    
    // Operators
    if (strcmp(token_name, "ASSIGN") == 0) return ASSIGN;
    if (strcmp(token_name, "PLUS") == 0) return PLUS;
    if (strcmp(token_name, "MINUS") == 0) return MINUS;
    if (strcmp(token_name, "MUL") == 0) return MUL;
    if (strcmp(token_name, "DIV") == 0) return DIV;
    if (strcmp(token_name, "MOD") == 0) return MOD;
    if (strcmp(token_name, "EQ") == 0) return EQ;
    if (strcmp(token_name, "NEQ") == 0) return NEQ;
    if (strcmp(token_name, "LT") == 0) return LT;
    if (strcmp(token_name, "GT") == 0) return GT;
    if (strcmp(token_name, "LE") == 0) return LE;
    if (strcmp(token_name, "GE") == 0) return GE;
    if (strcmp(token_name, "AND") == 0) return AND;
    if (strcmp(token_name, "OR") == 0) return OR;
    if (strcmp(token_name, "TILDE") == 0) return TILDE;
    
    // Separators
    if (strcmp(token_name, "LPAREN") == 0) return LPAREN;
    if (strcmp(token_name, "RPAREN") == 0) return RPAREN;
    if (strcmp(token_name, "LBRACK") == 0) return LBRACK;
    if (strcmp(token_name, "RBRACK") == 0) return RBRACK;
    if (strcmp(token_name, "LBRACE") == 0) return LBRACE;
    if (strcmp(token_name, "RBRACE") == 0) return RBRACE;
    if (strcmp(token_name, "COMMA") == 0) return COMMA;
    if (strcmp(token_name, "DOT") == 0) return DOT;
    if (strcmp(token_name, "SEMICOLON") == 0) return SEMICOLON;
    
    return -1; // Unknown token
}

// Initialize PIF file reading and symbol table
void init_pif_reader(const char* pif_filename) {
    pif_file = fopen(pif_filename, "r");
    if (!pif_file) {
        fprintf(stderr, "Cannot open PIF file: %s\n", pif_filename);
        exit(1);
    }
    
    // Initialize symbol table mapping
    init_st_mapping();
    
    // Derive ST filename from PIF filename (same directory)
    char st_filename[512];
    strcpy(st_filename, pif_filename);
    char* last_slash = strrchr(st_filename, '/');
    if (last_slash) {
        strcpy(last_slash + 1, "st.txt");
    } else {
        strcpy(st_filename, "st.txt");
    }
    
    // Read symbol table and build index-to-value mapping
    st_file = fopen(st_filename, "r");
    if (st_file) {
        char line[256];
        while (fgets(line, sizeof(line), st_file)) {
            int idx;
            char value[256];
            if (sscanf(line, "%d: %[^\n]", &idx, value) == 2) {
                set_st_value(idx, value);
            }
        }
        fclose(st_file);
        st_file = NULL;
    } else {
        fprintf(stderr, "Warning: Cannot open symbol table file: %s\n", st_filename);
    }
    
    pif_line_num = 0;
    yylineno = 0;
    current_token_name[0] = '\0';
}

// Cleanup
void cleanup_pif_reader(void) {
    if (pif_file) {
        fclose(pif_file);
        pif_file = NULL;
    }
    
    // Free symbol table values
    for (int i = 0; i <= st_max_index && i < MAX_ST_ENTRIES; i++) {
        if (st_values[i]) {
            free(st_values[i]);
            st_values[i] = NULL;
        }
    }
}

// Custom yylex that reads from PIF file
int yylex(void) {
    if (!pif_file) {
        strcpy(current_token_name, "EOF");
        yytext = current_token_name;
        return 0; // EOF
    }
    
    // Read next line from PIF
    if (fgets(current_line, sizeof(current_line), pif_file) == NULL) {
        fclose(pif_file);
        pif_file = NULL;
        return 0; // EOF
    }
    
    pif_line_num++;
    
    // Parse line: "TOKEN_NAME: position"
    char token_name[64];
    int pos;
    
    // Remove newline
    current_line[strcspn(current_line, "\n")] = 0;
    
    // Parse token name and position
    if (sscanf(current_line, "%63[^:]: %d", token_name, &pos) != 2) {
        fprintf(stderr, "Error parsing PIF line %d: %s\n", pif_line_num, current_line);
        return -1;
    }
    
    token_pos = pos;
    strncpy(current_token_name, token_name, sizeof(current_token_name) - 1);
    current_token_name[sizeof(current_token_name) - 1] = '\0';
    yytext = current_token_name;
    yylineno = pif_line_num;
    
    // Convert token name to token code
    int token_code = token_name_to_code(token_name);
    if (token_code == -1) {
        fprintf(stderr, "Unknown token in PIF: %s\n", token_name);
        return -1;
    }
    
    // For tokens with values (identifiers and constants), retrieve from symbol table
    if (token_code == IDENTIFIER || token_code == INT_CONST || 
        token_code == FLOAT_CONST || token_code == CHAR_CONST || 
        token_code == STRING_CONST) {
        
        if (pos >= 0) {
            const char* value = get_st_value(pos);
            if (value) {
                // For identifiers, strip the leading '!' if present
                if (token_code == IDENTIFIER && value[0] == '!') {
                    yylval.code = strdup(value + 1);  // Skip the '!' character
                } else {
                    yylval.code = strdup(value);
                }
            } else {
                fprintf(stderr, "Warning: No value found for token %s at index %d\n", token_name, pos);
                yylval.code = strdup("");
            }
        } else {
            yylval.code = strdup("");
        }
    }
    
    return token_code;
}

