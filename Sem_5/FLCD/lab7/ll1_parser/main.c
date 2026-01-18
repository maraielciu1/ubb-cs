#include "ll1_parser.h"
#include "pif_reader.h"
#include <stdio.h>
#include <stdlib.h>

// Token code to name mapping for output
const char* token_code_to_name(int code) {
    switch(code) {
        case TOKEN_START: return "START";
        case TOKEN_STOP: return "STOP";
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_READ: return "READ";
        case TOKEN_WRITE: return "WRITE";
        case TOKEN_INT: return "INT";
        case TOKEN_FLOAT: return "FLOAT";
        case TOKEN_STRING: return "STRING";
        case TOKEN_CHAR: return "CHAR";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_INT_CONST: return "INT_CONST";
        case TOKEN_FLOAT_CONST: return "FLOAT_CONST";
        case TOKEN_CHAR_CONST: return "CHAR_CONST";
        case TOKEN_STRING_CONST: return "STRING_CONST";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_MUL: return "MUL";
        case TOKEN_DIV: return "DIV";
        case TOKEN_MOD: return "MOD";
        case TOKEN_EQ: return "EQ";
        case TOKEN_NEQ: return "NEQ";
        case TOKEN_LT: return "LT";
        case TOKEN_GT: return "GT";
        case TOKEN_LE: return "LE";
        case TOKEN_GE: return "GE";
        case TOKEN_AND: return "AND";
        case TOKEN_OR: return "OR";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_EOF: return "$";
        default: return "UNKNOWN";
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <pif_file>\n", argv[0]);
        return 1;
    }
    
    printf("LL(1) Parser for Mini-DSL\n");
    printf("========================\n\n");
    
    // Read PIF file
    printf("Reading PIF file: %s\n", argv[1]);
    TokenStream* tokens = read_pif_file(argv[1]);
    if (!tokens) {
        return 1;
    }
    
    printf("Loaded %d tokens\n\n", tokens->count);
    
    // Create grammar
    printf("Creating grammar...\n");
    Grammar* grammar = create_grammar();
    printf("Grammar created with %d productions\n\n", grammar->num_productions);
    
    // Create LL(1) parser
    printf("Building LL(1) parser...\n");
    printf("Computing FIRST sets...\n");
    printf("Computing FOLLOW sets...\n");
    printf("Building parsing table...\n");
    LL1Parser* parser = create_ll1_parser(grammar);
    printf("LL(1) parser ready\n\n");
    
    // Parse input
    printf("Parsing input...\n");
    bool success = parse(parser, tokens->tokens, tokens->count);
    
    if (success) {
        printf("Parsing successful!\n\n");
        
        // Generate all three output formats
        printf("Generating output files...\n");
        
        // 2a: Productions string
        print_productions_string(parser, "out/productions.txt");
        printf("Productions string saved to: out/productions.txt\n");
        
        // 2b: Derivations string
        print_derivations_string(parser, "out/derivations.txt");
        printf("Derivations string saved to: out/derivations.txt\n");
        
        // 2c: Parse tree table (father-sibling)
        print_parse_tree_table(parser, "out/parse_tree_table.txt");
        printf("Parse tree table saved to: out/parse_tree_table.txt\n");
    } else {
        printf("Parsing failed!\n");
    }
    
    // Cleanup
    free_token_stream(tokens);
    free_parser(parser);
    free_grammar(grammar);
    
    return success ? 0 : 1;
}

