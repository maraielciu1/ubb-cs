#include "ll1_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Use the exported function from ll1_parser.c
extern ParseTreeNode* create_parse_node(LL1Parser* parser, const char* symbol, SymbolType type, int token_code);

static ParseTreeNode* create_node(LL1Parser* parser, const char* symbol, SymbolType type, int token_code) {
    return create_parse_node(parser, symbol, type, token_code);
}

// Create arithmetic expression grammar (E, T, F) for Requirement 1
// Grammar: E → E + T | T
//          T → T * F | F  
//          F → (E) | a
// Converted to LL(1) form (right recursion):
//          E → T E'
//          E' → + T E' | ε
//          T → F T'
//          T' → * F T' | ε
//          F → (E) | a

Grammar* create_arithmetic_grammar(void) {
    Grammar* g = (Grammar*)calloc(1, sizeof(Grammar));
    
    // Initialize start symbol
    strcpy(g->start_symbol, "E");
    
    // Add non-terminals
    strcpy(g->non_terminals[g->num_non_terminals++].name, "E");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "E_prime");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "T");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "T_prime");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "F");
    
    // Add productions (converted to right recursion for LL(1))
    int prod_num = 1;
    
    // E → T E'
    const char* p1[] = {"T", "E_prime"};
    add_production(g, "E", p1, 2, prod_num++);
    
    // E' → + T E' | ε
    const char* p2a[] = {"+", "T", "E_prime"};
    add_production(g, "E_prime", p2a, 3, prod_num++);
    const char* p2b[] = {"epsilon"};
    add_production(g, "E_prime", p2b, 1, prod_num++);
    
    // T → F T'
    const char* p3[] = {"F", "T_prime"};
    add_production(g, "T", p3, 2, prod_num++);
    
    // T' → * F T' | ε
    const char* p4a[] = {"*", "F", "T_prime"};
    add_production(g, "T_prime", p4a, 3, prod_num++);
    const char* p4b[] = {"epsilon"};
    add_production(g, "T_prime", p4b, 1, prod_num++);
    
    // F → (E) | a
    const char* p5a[] = {"(", "E", ")"};
    add_production(g, "F", p5a, 3, prod_num++);
    const char* p5b[] = {"a"};
    add_production(g, "F", p5b, 1, prod_num++);
    
    return g;
}

// Convert sequence string to token array
// Sequence format: "a * (a + a)" -> tokens: a, *, (, a, +, a, )
int* sequence_to_tokens(const char* sequence, int* token_count) {
    // Simple tokenizer for arithmetic expressions
    // Tokens: 'a' = 1, '+' = 2, '*' = 3, '(' = 4, ')' = 5, EOF = 0
    int* tokens = (int*)malloc(100 * sizeof(int));
    *token_count = 0;
    
    for (int i = 0; sequence[i] != '\0'; i++) {
        if (sequence[i] == ' ') continue;  // Skip spaces
        
        switch(sequence[i]) {
            case 'a': tokens[(*token_count)++] = 1; break;
            case '+': tokens[(*token_count)++] = 2; break;
            case '*': tokens[(*token_count)++] = 3; break;
            case '(': tokens[(*token_count)++] = 4; break;
            case ')': tokens[(*token_count)++] = 5; break;
        }
    }
    tokens[(*token_count)++] = 0;  // EOF
    
    return tokens;
}

// Token code to name for arithmetic grammar
const char* arithmetic_token_name(int code) {
    switch(code) {
        case 1: return "a";
        case 2: return "+";
        case 3: return "*";
        case 4: return "(";
        case 5: return ")";
        case 0: return "$";
        default: return "UNKNOWN";
    }
}

// Check if symbol matches token for arithmetic grammar
static bool arithmetic_is_terminal(const char* name) {
    return (strcmp(name, "a") == 0 || strcmp(name, "+") == 0 || 
            strcmp(name, "*") == 0 || strcmp(name, "(") == 0 || 
            strcmp(name, ")") == 0);
}

// Get token code for arithmetic terminal
static int arithmetic_token_code(const char* name) {
    if (strcmp(name, "a") == 0) return 1;
    if (strcmp(name, "+") == 0) return 2;
    if (strcmp(name, "*") == 0) return 3;
    if (strcmp(name, "(") == 0) return 4;
    if (strcmp(name, ")") == 0) return 5;
    return -1;
}

// Check if a symbol (terminal or non-terminal) can start with a given token for arithmetic grammar
static bool arithmetic_can_start_with(LL1Parser* parser, const char* sym, int token) {
    if (arithmetic_is_terminal(sym)) {
        int sym_token = arithmetic_token_code(sym);
        return sym_token == token;
    }
    
    if (strcmp(sym, "epsilon") == 0) {
        return false;
    }
    
    // Non-terminal - recursively check if it can derive the token
    Grammar* g = parser->grammar;
    
    // Direct mappings for arithmetic grammar
    if (strcmp(sym, "E") == 0 || strcmp(sym, "T") == 0 || strcmp(sym, "F") == 0) {
        // E, T, F can all start with 'a' or '('
        return (token == 1 || token == 4);  // 'a' or '('
    }
    
    if (strcmp(sym, "E_prime") == 0) {
        // E' can start with '+' or be epsilon (FOLLOW(E') = {$, )})
        return (token == 2 || token == 0 || token == 5);  // '+' or '$' or ')'
    }
    
    if (strcmp(sym, "T_prime") == 0) {
        // T' can start with '*' or be epsilon (FOLLOW(T') = {+, $, )})
        return (token == 3 || token == 2 || token == 0 || token == 5);  // '*' or '+' or '$' or ')'
    }
    
    // Check productions for this non-terminal
    for (int i = 0; i < g->num_productions; i++) {
        if (strcmp(g->productions[i].lhs, sym) == 0) {
            Production* p = &g->productions[i];
            if (p->rhs_len > 0) {
                if (arithmetic_can_start_with(parser, p->rhs[0], token)) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

// Find production for arithmetic grammar
static Production* find_arithmetic_production(LL1Parser* parser, const char* nt, int token) {
    Grammar* g = parser->grammar;
    
    Production* epsilon_prod = NULL;
    
    // First pass: try all non-epsilon productions
    for (int i = 0; i < g->num_productions; i++) {
        if (strcmp(g->productions[i].lhs, nt) != 0) continue;
        
        Production* p = &g->productions[i];
        
        // Check if production can start with this token
        if (p->rhs_len == 0) continue;
        
        // Check first symbol
        const char* first_sym = p->rhs[0];
        
        // Remember epsilon production for later
        if (strcmp(first_sym, "epsilon") == 0) {
            epsilon_prod = p;
            continue;
        }
        
        // Check if this production can start with the token
        if (arithmetic_can_start_with(parser, first_sym, token)) {
            return p;
        }
    }
    
    // Second pass: if no non-epsilon production matched, check epsilon
    if (epsilon_prod) {
        // Check if token is in FOLLOW(nt)
        if (strcmp(nt, "E_prime") == 0) {
            // FOLLOW(E') = {$, )}
            if (token == 0 || token == 5) {  // '$' or ')'
                return epsilon_prod;
            }
        } else if (strcmp(nt, "T_prime") == 0) {
            // FOLLOW(T') = {+, $, )}
            if (token == 2 || token == 0 || token == 5) {  // '+' or '$' or ')'
                return epsilon_prod;
            }
        }
    }
    
    return NULL;
}

// Parse arithmetic expression using LL(1)
bool parse_arithmetic(LL1Parser* parser, int* input_tokens, int input_len) {
    Grammar* g = parser->grammar;
    
    // Stack for LL(1) parsing
    struct {
        ParseTreeNode* nodes[MAX_STACK_SIZE];
        int top;
    } stack;
    stack.top = -1;
    
    // Create root node
    ParseTreeNode* root = create_node(parser, g->start_symbol, SYM_NON_TERMINAL, -1);
    parser->parse_tree = root;
    stack.nodes[++stack.top] = root;
    
    int input_pos = 0;
    
    printf("Parsing sequence...\n");
    printf("Stack operations:\n");
    
    while (stack.top >= 0 && input_pos < input_len) {
        ParseTreeNode* top_node = stack.nodes[stack.top];
        int current_token = input_tokens[input_pos];
        
        if (arithmetic_is_terminal(top_node->symbol)) {
            // Terminal on stack - match with input
            int expected_token = arithmetic_token_code(top_node->symbol);
            if (expected_token == current_token) {
                top_node->token_code = current_token;
                printf("  Match: %s\n", top_node->symbol);
                stack.top--;  // Pop
                input_pos++;  // Advance input
            } else {
                fprintf(stderr, "Syntax error: expected %s, got token %d\n", 
                        top_node->symbol, current_token);
                return false;
            }
        } else if (strcmp(top_node->symbol, "epsilon") == 0) {
            // Epsilon - just pop
            printf("  Epsilon production\n");
            stack.top--;
        } else {
            // Non-terminal - expand using production
            Production* prod = find_arithmetic_production(parser, top_node->symbol, current_token);
            if (!prod) {
                fprintf(stderr, "Syntax error: no production for %s with token %d\n",
                        top_node->symbol, current_token);
                return false;
            }
            
            printf("  Apply: %s -> ", prod->lhs);
            for (int i = 0; i < prod->rhs_len; i++) {
                printf("%s ", prod->rhs[i]);
            }
            printf("(production %d)\n", prod->prod_num);
            
            // Pop non-terminal
            stack.top--;
            
            // Create children and push to stack
            ParseTreeNode* children[20];
            int num_children = 0;
            
            for (int i = 0; i < prod->rhs_len; i++) {
                const char* sym = prod->rhs[i];
                SymbolType sym_type = arithmetic_is_terminal(sym) ? SYM_TERMINAL : SYM_NON_TERMINAL;
                int sym_token = arithmetic_is_terminal(sym) ? arithmetic_token_code(sym) : -1;
                
                ParseTreeNode* child = create_node(parser, sym, sym_type, sym_token);
                child->parent = top_node;
                children[num_children++] = child;
            }
            
            // Link children as siblings
            if (num_children > 0) {
                top_node->first_child = children[0];
                for (int i = 0; i < num_children - 1; i++) {
                    children[i]->next_sibling = children[i + 1];
                }
            }
            
            // Push to stack in reverse order
            for (int i = prod->rhs_len - 1; i >= 0; i--) {
                stack.nodes[++stack.top] = children[i];
            }
        }
    }
    
    // Check if parsing completed successfully
    if (stack.top < 0 && input_pos >= input_len - 1) {
        return true;
    }
    
    return false;
}

