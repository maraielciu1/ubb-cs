#include "pif_reader.h"
#include "ll1_parser.h"

// Convert token name to token code
int token_name_to_code(const char* token_name) {
    if (strcmp(token_name, "START") == 0) return TOKEN_START;
    if (strcmp(token_name, "STOP") == 0) return TOKEN_STOP;
    if (strcmp(token_name, "IF") == 0) return TOKEN_IF;
    if (strcmp(token_name, "ELSE") == 0) return TOKEN_ELSE;
    if (strcmp(token_name, "WHILE") == 0) return TOKEN_WHILE;
    if (strcmp(token_name, "READ") == 0) return TOKEN_READ;
    if (strcmp(token_name, "WRITE") == 0) return TOKEN_WRITE;
    if (strcmp(token_name, "INT") == 0) return TOKEN_INT;
    if (strcmp(token_name, "FLOAT") == 0) return TOKEN_FLOAT;
    if (strcmp(token_name, "STRING") == 0) return TOKEN_STRING;
    if (strcmp(token_name, "CHAR") == 0) return TOKEN_CHAR;
    if (strcmp(token_name, "IDENTIFIER") == 0) return TOKEN_IDENTIFIER;
    if (strcmp(token_name, "INT_CONST") == 0) return TOKEN_INT_CONST;
    if (strcmp(token_name, "FLOAT_CONST") == 0) return TOKEN_FLOAT_CONST;
    if (strcmp(token_name, "CHAR_CONST") == 0) return TOKEN_CHAR_CONST;
    if (strcmp(token_name, "STRING_CONST") == 0) return TOKEN_STRING_CONST;
    if (strcmp(token_name, "ASSIGN") == 0) return TOKEN_ASSIGN;
    if (strcmp(token_name, "PLUS") == 0) return TOKEN_PLUS;
    if (strcmp(token_name, "MINUS") == 0) return TOKEN_MINUS;
    if (strcmp(token_name, "MUL") == 0) return TOKEN_MUL;
    if (strcmp(token_name, "DIV") == 0) return TOKEN_DIV;
    if (strcmp(token_name, "MOD") == 0) return TOKEN_MOD;
    if (strcmp(token_name, "EQ") == 0) return TOKEN_EQ;
    if (strcmp(token_name, "NEQ") == 0) return TOKEN_NEQ;
    if (strcmp(token_name, "LT") == 0) return TOKEN_LT;
    if (strcmp(token_name, "GT") == 0) return TOKEN_GT;
    if (strcmp(token_name, "LE") == 0) return TOKEN_LE;
    if (strcmp(token_name, "GE") == 0) return TOKEN_GE;
    if (strcmp(token_name, "AND") == 0) return TOKEN_AND;
    if (strcmp(token_name, "OR") == 0) return TOKEN_OR;
    if (strcmp(token_name, "LPAREN") == 0) return TOKEN_LPAREN;
    if (strcmp(token_name, "RPAREN") == 0) return TOKEN_RPAREN;
    if (strcmp(token_name, "LBRACE") == 0) return TOKEN_LBRACE;
    if (strcmp(token_name, "RBRACE") == 0) return TOKEN_RBRACE;
    if (strcmp(token_name, "SEMICOLON") == 0) return TOKEN_SEMICOLON;
    if (strcmp(token_name, "COMMA") == 0) return TOKEN_COMMA;
    return -1;
}

// Read PIF file
TokenStream* read_pif_file(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Cannot open PIF file: %s\n", filename);
        return NULL;
    }
    
    TokenStream* ts = (TokenStream*)calloc(1, sizeof(TokenStream));
    ts->capacity = 1000;
    ts->tokens = (int*)malloc(ts->capacity * sizeof(int));
    ts->count = 0;
    
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        // Parse line: "TOKEN_NAME: position"
        char token_name[64];
        int pos;
        
        line[strcspn(line, "\n")] = 0;
        
        if (sscanf(line, "%63[^:]: %d", token_name, &pos) == 2) {
            int token_code = token_name_to_code(token_name);
            if (token_code >= 0) {
                if (ts->count >= ts->capacity) {
                    ts->capacity *= 2;
                    ts->tokens = (int*)realloc(ts->tokens, ts->capacity * sizeof(int));
                }
                ts->tokens[ts->count++] = token_code;
            }
        }
    }
    
    // Add EOF
    if (ts->count >= ts->capacity) {
        ts->capacity *= 2;
        ts->tokens = (int*)realloc(ts->tokens, ts->capacity * sizeof(int));
    }
    ts->tokens[ts->count++] = TOKEN_EOF;
    
    fclose(f);
    return ts;
}

void free_token_stream(TokenStream* ts) {
    if (ts) {
        free(ts->tokens);
        free(ts);
    }
}

