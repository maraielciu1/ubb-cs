#include "ll1_parser.h"
#include "pif_reader.h"
#include <limits.h>

// Forward declaration
extern int token_name_to_code(const char* token_name);

// Helper function to check if symbol is terminal
static bool is_terminal(const char* name) {
    // Non-terminals are lowercase or contain underscores, terminals are uppercase or special
    if (name[0] >= 'A' && name[0] <= 'Z') {
        return true;
    }
    // Check for special terminal symbols
    if (strcmp(name, "IDENTIFIER") == 0 || strcmp(name, "INT_CONST") == 0 ||
        strcmp(name, "FLOAT_CONST") == 0 || strcmp(name, "CHAR_CONST") == 0 ||
        strcmp(name, "STRING_CONST") == 0) {
        return true;
    }
    return false;
}

// Create grammar with LL(1) compatible rules (right recursion)
Grammar* create_grammar(void) {
    Grammar* g = (Grammar*)calloc(1, sizeof(Grammar));
    
    // Initialize start symbol
    strcpy(g->start_symbol, "program");
    
    // Add non-terminals
    strcpy(g->non_terminals[g->num_non_terminals++].name, "program");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "statement_list");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "statement");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "assignment_statement");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "if_statement");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "if_tail");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "while_statement");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "io_statement");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "declaration_statement");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "type");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "expression");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "expression_prime");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "term");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "term_prime");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "factor");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "constant");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "condition");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "condition_prime");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "relational_operator");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "addition_operator");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "multiplication_operator");
    strcpy(g->non_terminals[g->num_non_terminals++].name, "logical_operator");
    
    // Add productions (converted to right recursion for LL(1))
    int prod_num = 1;
    
    // program -> START statement_list STOP
    const char* p1[] = {"START", "statement_list", "STOP"};
    add_production(g, "program", p1, 3, prod_num++);
    
    // statement_list -> statement statement_list | epsilon
    const char* p2[] = {"statement", "statement_list"};
    add_production(g, "statement_list", p2, 2, prod_num++);
    const char* p2e[] = {"epsilon"};
    add_production(g, "statement_list", p2e, 1, prod_num++);
    
    // statement -> assignment_statement | if_statement | while_statement | io_statement | declaration_statement
    const char* p3a[] = {"assignment_statement"};
    add_production(g, "statement", p3a, 1, prod_num++);
    const char* p3b[] = {"if_statement"};
    add_production(g, "statement", p3b, 1, prod_num++);
    const char* p3c[] = {"while_statement"};
    add_production(g, "statement", p3c, 1, prod_num++);
    const char* p3d[] = {"io_statement"};
    add_production(g, "statement", p3d, 1, prod_num++);
    const char* p3e[] = {"declaration_statement"};
    add_production(g, "statement", p3e, 1, prod_num++);
    
    // assignment_statement -> IDENTIFIER ASSIGN expression SEMICOLON
    const char* p4[] = {"IDENTIFIER", "ASSIGN", "expression", "SEMICOLON"};
    add_production(g, "assignment_statement", p4, 4, prod_num++);
    
    // if_statement -> IF LPAREN condition RPAREN LBRACE statement_list RBRACE if_tail
    // Factored to make it LL(1)
    const char* p5[] = {"IF", "LPAREN", "condition", "RPAREN", "LBRACE", "statement_list", "RBRACE", "if_tail"};
    add_production(g, "if_statement", p5, 8, prod_num++);
    
    // if_tail -> ELSE LBRACE statement_list RBRACE | epsilon
    const char* p5a[] = {"ELSE", "LBRACE", "statement_list", "RBRACE"};
    add_production(g, "if_tail", p5a, 4, prod_num++);
    const char* p5b[] = {"epsilon"};
    add_production(g, "if_tail", p5b, 1, prod_num++);
    
    // while_statement -> WHILE LPAREN condition RPAREN LBRACE statement_list RBRACE
    const char* p6[] = {"WHILE", "LPAREN", "condition", "RPAREN", "LBRACE", "statement_list", "RBRACE"};
    add_production(g, "while_statement", p6, 7, prod_num++);
    
    // io_statement -> WRITE LPAREN expression RPAREN SEMICOLON | READ LPAREN IDENTIFIER RPAREN SEMICOLON
    const char* p7a[] = {"WRITE", "LPAREN", "expression", "RPAREN", "SEMICOLON"};
    add_production(g, "io_statement", p7a, 5, prod_num++);
    const char* p7b[] = {"READ", "LPAREN", "IDENTIFIER", "RPAREN", "SEMICOLON"};
    add_production(g, "io_statement", p7b, 5, prod_num++);
    
    // declaration_statement -> type IDENTIFIER ASSIGN expression SEMICOLON
    const char* p8[] = {"type", "IDENTIFIER", "ASSIGN", "expression", "SEMICOLON"};
    add_production(g, "declaration_statement", p8, 5, prod_num++);
    
    // type -> INT | FLOAT | CHAR | STRING
    const char* p9a[] = {"INT"};
    add_production(g, "type", p9a, 1, prod_num++);
    const char* p9b[] = {"FLOAT"};
    add_production(g, "type", p9b, 1, prod_num++);
    const char* p9c[] = {"CHAR"};
    add_production(g, "type", p9c, 1, prod_num++);
    const char* p9d[] = {"STRING"};
    add_production(g, "type", p9d, 1, prod_num++);
    
    // expression -> term expression_prime (right recursion)
    const char* p10[] = {"term", "expression_prime"};
    add_production(g, "expression", p10, 2, prod_num++);
    
    // expression_prime -> addition_operator term expression_prime | epsilon
    const char* p11a[] = {"addition_operator", "term", "expression_prime"};
    add_production(g, "expression_prime", p11a, 3, prod_num++);
    const char* p11b[] = {"epsilon"};
    add_production(g, "expression_prime", p11b, 1, prod_num++);
    
    // term -> factor term_prime (right recursion)
    const char* p12[] = {"factor", "term_prime"};
    add_production(g, "term", p12, 2, prod_num++);
    
    // term_prime -> multiplication_operator factor term_prime | epsilon
    const char* p13a[] = {"multiplication_operator", "factor", "term_prime"};
    add_production(g, "term_prime", p13a, 3, prod_num++);
    const char* p13b[] = {"epsilon"};
    add_production(g, "term_prime", p13b, 1, prod_num++);
    
    // factor -> IDENTIFIER | constant | LPAREN expression RPAREN
    const char* p14a[] = {"IDENTIFIER"};
    add_production(g, "factor", p14a, 1, prod_num++);
    const char* p14b[] = {"constant"};
    add_production(g, "factor", p14b, 1, prod_num++);
    const char* p14c[] = {"LPAREN", "expression", "RPAREN"};
    add_production(g, "factor", p14c, 3, prod_num++);
    
    // constant -> INT_CONST | FLOAT_CONST | CHAR_CONST | STRING_CONST
    const char* p15a[] = {"INT_CONST"};
    add_production(g, "constant", p15a, 1, prod_num++);
    const char* p15b[] = {"FLOAT_CONST"};
    add_production(g, "constant", p15b, 1, prod_num++);
    const char* p15c[] = {"CHAR_CONST"};
    add_production(g, "constant", p15c, 1, prod_num++);
    const char* p15d[] = {"STRING_CONST"};
    add_production(g, "constant", p15d, 1, prod_num++);
    
    // condition -> expression relational_operator expression condition_prime | LPAREN condition RPAREN condition_prime
    const char* p16a[] = {"expression", "relational_operator", "expression", "condition_prime"};
    add_production(g, "condition", p16a, 4, prod_num++);
    const char* p16b[] = {"LPAREN", "condition", "RPAREN", "condition_prime"};
    add_production(g, "condition", p16b, 4, prod_num++);
    
    // condition_prime -> logical_operator condition condition_prime | epsilon
    const char* p17a[] = {"logical_operator", "condition", "condition_prime"};
    add_production(g, "condition_prime", p17a, 3, prod_num++);
    const char* p17b[] = {"epsilon"};
    add_production(g, "condition_prime", p17b, 1, prod_num++);
    
    // relational_operator -> EQ | NEQ | LT | GT | LE | GE
    const char* p18a[] = {"EQ"};
    add_production(g, "relational_operator", p18a, 1, prod_num++);
    const char* p18b[] = {"NEQ"};
    add_production(g, "relational_operator", p18b, 1, prod_num++);
    const char* p18c[] = {"LT"};
    add_production(g, "relational_operator", p18c, 1, prod_num++);
    const char* p18d[] = {"GT"};
    add_production(g, "relational_operator", p18d, 1, prod_num++);
    const char* p18e[] = {"LE"};
    add_production(g, "relational_operator", p18e, 1, prod_num++);
    const char* p18f[] = {"GE"};
    add_production(g, "relational_operator", p18f, 1, prod_num++);
    
    // addition_operator -> PLUS | MINUS
    const char* p19a[] = {"PLUS"};
    add_production(g, "addition_operator", p19a, 1, prod_num++);
    const char* p19b[] = {"MINUS"};
    add_production(g, "addition_operator", p19b, 1, prod_num++);
    
    // multiplication_operator -> MUL | DIV | MOD
    const char* p20a[] = {"MUL"};
    add_production(g, "multiplication_operator", p20a, 1, prod_num++);
    const char* p20b[] = {"DIV"};
    add_production(g, "multiplication_operator", p20b, 1, prod_num++);
    const char* p20c[] = {"MOD"};
    add_production(g, "multiplication_operator", p20c, 1, prod_num++);
    
    // logical_operator -> AND | OR
    const char* p21a[] = {"AND"};
    add_production(g, "logical_operator", p21a, 1, prod_num++);
    const char* p21b[] = {"OR"};
    add_production(g, "logical_operator", p21b, 1, prod_num++);
    
    return g;
}

// Add a production to grammar
void add_production(Grammar* g, const char* lhs, const char** rhs, int rhs_len, int prod_num) {
    if (g->num_productions >= MAX_PRODUCTIONS) return;
    
    Production* p = &g->productions[g->num_productions];
    strcpy(p->lhs, lhs);
    p->rhs_len = rhs_len;
    p->prod_num = prod_num;
    
    for (int i = 0; i < rhs_len; i++) {
        strcpy(p->rhs[i], rhs[i]);
    }
    
    g->num_productions++;
}

// Get non-terminal index
int get_non_terminal_index(Grammar* g, const char* name) {
    for (int i = 0; i < g->num_non_terminals; i++) {
        if (strcmp(g->non_terminals[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Get terminal index by token code
int get_terminal_index(Grammar* g, int token_code) {
    // Map token codes to terminal indices
    int base = TOKEN_START;
    if (token_code >= base && token_code < base + 35) {
        return token_code - base;
    }
    return -1;
}

// Compute FIRST sets
void compute_first_sets(LL1Parser* parser) {
    Grammar* g = parser->grammar;
    bool changed = true;
    
    // Initialize FIRST sets
    for (int i = 0; i < g->num_non_terminals; i++) {
        parser->first_sets[i] = (int*)calloc(MAX_TERMINALS, sizeof(int));
    }
    
    // Iterate until no changes
    while (changed) {
        changed = false;
        
        for (int i = 0; i < g->num_productions; i++) {
            Production* p = &g->productions[i];
            int nt_idx = get_non_terminal_index(g, p->lhs);
            if (nt_idx == -1) continue;
            
            // For each production A -> X1 X2 ... Xn
            bool has_epsilon = true;
            for (int j = 0; j < p->rhs_len && has_epsilon; j++) {
                const char* sym = p->rhs[j];
                
                if (strcmp(sym, "epsilon") == 0) {
                    // Add epsilon to FIRST(A)
                    if (parser->first_sets[nt_idx][MAX_TERMINALS - 1] == 0) {
                        parser->first_sets[nt_idx][MAX_TERMINALS - 1] = 1;
                        changed = true;
                    }
                    break;
                } else if (is_terminal(sym)) {
                    // Terminal: add to FIRST(A)
                    int term_idx = -1;
                    // Find terminal index (simplified - would need proper mapping)
                    for (int k = 0; k < g->num_terminals; k++) {
                        if (strcmp(g->terminals[k].name, sym) == 0) {
                            term_idx = k;
                            break;
                        }
                    }
                    if (term_idx >= 0 && parser->first_sets[nt_idx][term_idx] == 0) {
                        parser->first_sets[nt_idx][term_idx] = 1;
                        changed = true;
                    }
                    has_epsilon = false;
                    break;
                } else {
                    // Non-terminal: add FIRST(Xj) - {epsilon} to FIRST(A)
                    int sym_nt_idx = get_non_terminal_index(g, sym);
                    if (sym_nt_idx >= 0) {
                        for (int k = 0; k < MAX_TERMINALS - 1; k++) {
                            if (parser->first_sets[sym_nt_idx][k] == 1 &&
                                parser->first_sets[nt_idx][k] == 0) {
                                parser->first_sets[nt_idx][k] = 1;
                                changed = true;
                            }
                        }
                        // If epsilon not in FIRST(Xj), stop
                        if (parser->first_sets[sym_nt_idx][MAX_TERMINALS - 1] == 0) {
                            has_epsilon = false;
                            break;
                        }
                    }
                }
            }
            
            // If all symbols can derive epsilon, add epsilon to FIRST(A)
            if (has_epsilon && parser->first_sets[nt_idx][MAX_TERMINALS - 1] == 0) {
                parser->first_sets[nt_idx][MAX_TERMINALS - 1] = 1;
                changed = true;
            }
        }
    }
}

// Compute FOLLOW sets
void compute_follow_sets(LL1Parser* parser) {
    Grammar* g = parser->grammar;
    bool changed = true;
    
    // Initialize FOLLOW sets
    for (int i = 0; i < g->num_non_terminals; i++) {
        parser->follow_sets[i] = (int*)calloc(MAX_TERMINALS, sizeof(int));
    }
    
    // Add $ (EOF) to FOLLOW(start_symbol)
    int start_idx = get_non_terminal_index(g, g->start_symbol);
    if (start_idx >= 0) {
        parser->follow_sets[start_idx][MAX_TERMINALS - 1] = 1;  // Using last slot for EOF
    }
    
    // Iterate until no changes
    while (changed) {
        changed = false;
        
        for (int i = 0; i < g->num_productions; i++) {
            Production* p = &g->productions[i];
            
            // For each production A -> X1 X2 ... Xn
            for (int j = 0; j < p->rhs_len; j++) {
                const char* sym = p->rhs[j];
                
                if (!is_terminal(sym) && strcmp(sym, "epsilon") != 0) {
                    int sym_nt_idx = get_non_terminal_index(g, sym);
                    if (sym_nt_idx == -1) continue;
                    
                    // For each symbol Xj that is a non-terminal
                    bool has_epsilon = true;
                    for (int k = j + 1; k < p->rhs_len && has_epsilon; k++) {
                        const char* next_sym = p->rhs[k];
                        
                        if (is_terminal(next_sym)) {
                            // Add terminal to FOLLOW(Xj)
                            // Simplified - would need proper terminal mapping
                            has_epsilon = false;
                            break;
                        } else {
                            // Add FIRST(Xk) - {epsilon} to FOLLOW(Xj)
                            int next_nt_idx = get_non_terminal_index(g, next_sym);
                            if (next_nt_idx >= 0) {
                                for (int m = 0; m < MAX_TERMINALS - 1; m++) {
                                    if (parser->first_sets[next_nt_idx][m] == 1 &&
                                        parser->follow_sets[sym_nt_idx][m] == 0) {
                                        parser->follow_sets[sym_nt_idx][m] = 1;
                                        changed = true;
                                    }
                                }
                                // If epsilon not in FIRST(Xk), stop
                                if (parser->first_sets[next_nt_idx][MAX_TERMINALS - 1] == 0) {
                                    has_epsilon = false;
                                    break;
                                }
                            }
                        }
                    }
                    
                    // If all symbols after Xj can derive epsilon, add FOLLOW(A) to FOLLOW(Xj)
                    if (has_epsilon) {
                        int lhs_nt_idx = get_non_terminal_index(g, p->lhs);
                        if (lhs_nt_idx >= 0) {
                            for (int m = 0; m < MAX_TERMINALS; m++) {
                                if (parser->follow_sets[lhs_nt_idx][m] == 1 &&
                                    parser->follow_sets[sym_nt_idx][m] == 0) {
                                    parser->follow_sets[sym_nt_idx][m] = 1;
                                    changed = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// Build LL(1) parsing table
void build_parsing_table(LL1Parser* parser) {
    Grammar* g = parser->grammar;
    
    // Allocate parsing table
    parser->parsing_table = (bool**)calloc(g->num_non_terminals, sizeof(bool*));
    for (int i = 0; i < g->num_non_terminals; i++) {
        parser->parsing_table[i] = (bool*)calloc(MAX_TERMINALS, sizeof(bool));
    }
    
    // For each production A -> alpha
    for (int i = 0; i < g->num_productions; i++) {
        Production* p = &g->productions[i];
        int nt_idx = get_non_terminal_index(g, p->lhs);
        if (nt_idx == -1) continue;
        
        // Compute FIRST(alpha)
        bool first_has_epsilon = false;
        for (int j = 0; j < p->rhs_len; j++) {
            const char* sym = p->rhs[j];
            
            if (strcmp(sym, "epsilon") == 0) {
                first_has_epsilon = true;
                break;
            } else if (is_terminal(sym)) {
                // Add production to M[A, a] for each a in FIRST(alpha)
                // Simplified - would need proper terminal mapping
                break;
            } else {
                int sym_nt_idx = get_non_terminal_index(g, sym);
                if (sym_nt_idx >= 0) {
                    // Add terminals from FIRST(sym) to table
                    for (int k = 0; k < MAX_TERMINALS - 1; k++) {
                        if (parser->first_sets[sym_nt_idx][k] == 1) {
                            parser->parsing_table[nt_idx][k] = true;
                        }
                    }
                    if (parser->first_sets[sym_nt_idx][MAX_TERMINALS - 1] == 0) {
                        break;  // No epsilon
                    }
                }
            }
        }
        
        // If epsilon in FIRST(alpha), add to M[A, b] for each b in FOLLOW(A)
        if (first_has_epsilon || (p->rhs_len == 1 && strcmp(p->rhs[0], "epsilon") == 0)) {
            for (int k = 0; k < MAX_TERMINALS; k++) {
                if (parser->follow_sets[nt_idx][k] == 1) {
                    parser->parsing_table[nt_idx][k] = true;
                }
            }
        }
    }
}

// Create LL(1) parser
LL1Parser* create_ll1_parser(Grammar* g) {
    LL1Parser* parser = (LL1Parser*)calloc(1, sizeof(LL1Parser));
    parser->grammar = g;
    parser->next_node_id = 1;
    
    compute_first_sets(parser);
    compute_follow_sets(parser);
    build_parsing_table(parser);
    
    return parser;
}

// Stack for LL(1) parsing
typedef struct {
    ParseTreeNode* nodes[MAX_STACK_SIZE];
    int top;
} ParseStack;

// Create parse tree node (exported for use by arithmetic grammar)
ParseTreeNode* create_parse_node(LL1Parser* parser, const char* symbol, SymbolType type, int token_code) {
    ParseTreeNode* node = (ParseTreeNode*)calloc(1, sizeof(ParseTreeNode));
    strcpy(node->symbol, symbol);
    node->type = type;
    node->token_code = token_code;
    node->node_id = parser->next_node_id++;
    node->production_num = -1;  // Initialize to -1 (not set)
    node->parent = NULL;
    node->first_child = NULL;
    node->next_sibling = NULL;
    return node;
}

// Internal alias for use within this file
static ParseTreeNode* create_node(LL1Parser* parser, const char* symbol, SymbolType type, int token_code) {
    return create_parse_node(parser, symbol, type, token_code);
}

// Check if a symbol (terminal or non-terminal) can start with a given token
static bool can_start_with(LL1Parser* parser, const char* sym, int token) {
    if (is_terminal(sym)) {
        int sym_token = token_name_to_code(sym);
        return sym_token == token;
    }
    
    if (strcmp(sym, "epsilon") == 0) {
        return false;
    }
    
    // Non-terminal - recursively check if it can derive the token
    Grammar* g = parser->grammar;
    
    // Direct mappings for common cases
    if (strcmp(sym, "constant") == 0) {
        return (token == TOKEN_INT_CONST || token == TOKEN_FLOAT_CONST || 
                token == TOKEN_CHAR_CONST || token == TOKEN_STRING_CONST);
    }
    
    if (strcmp(sym, "factor") == 0) {
        return (token == TOKEN_IDENTIFIER || token == TOKEN_INT_CONST || 
                token == TOKEN_FLOAT_CONST || token == TOKEN_CHAR_CONST || 
                token == TOKEN_STRING_CONST || token == TOKEN_LPAREN);
    }
    
    if (strcmp(sym, "term") == 0 || strcmp(sym, "expression") == 0) {
        // Can start with anything that factor can start with
        return can_start_with(parser, "factor", token);
    }
    
    // addition_operator can start with PLUS or MINUS
    if (strcmp(sym, "addition_operator") == 0) {
        return (token == TOKEN_PLUS || token == TOKEN_MINUS);
    }
    
    // multiplication_operator can start with MUL, DIV, or MOD
    if (strcmp(sym, "multiplication_operator") == 0) {
        return (token == TOKEN_MUL || token == TOKEN_DIV || token == TOKEN_MOD);
    }
    
    // relational_operator can start with EQ, NEQ, LT, GT, LE, GE
    if (strcmp(sym, "relational_operator") == 0) {
        return (token == TOKEN_EQ || token == TOKEN_NEQ || token == TOKEN_LT ||
                token == TOKEN_GT || token == TOKEN_LE || token == TOKEN_GE);
    }
    
    // logical_operator can start with AND or OR
    if (strcmp(sym, "logical_operator") == 0) {
        return (token == TOKEN_AND || token == TOKEN_OR);
    }
    
    // type can start with INT, FLOAT, CHAR, STRING
    if (strcmp(sym, "type") == 0) {
        return (token == TOKEN_INT || token == TOKEN_FLOAT || 
                token == TOKEN_CHAR || token == TOKEN_STRING);
    }
    
    // statement can start with keywords (IF, WHILE, READ, WRITE) or types (for declarations) or IDENTIFIER (for assignment)
    if (strcmp(sym, "statement") == 0) {
        return (token == TOKEN_IDENTIFIER || token == TOKEN_IF || token == TOKEN_WHILE ||
                token == TOKEN_READ || token == TOKEN_WRITE ||
                token == TOKEN_INT || token == TOKEN_FLOAT || 
                token == TOKEN_CHAR || token == TOKEN_STRING);
    }
    
    // condition can start with expression or LPAREN
    if (strcmp(sym, "condition") == 0) {
        return (can_start_with(parser, "expression", token) || token == TOKEN_LPAREN);
    }
    
    // condition_prime can start with logical_operator (AND/OR) or be epsilon
    if (strcmp(sym, "condition_prime") == 0) {
        return (token == TOKEN_AND || token == TOKEN_OR ||
                token == TOKEN_RPAREN || token == TOKEN_RBRACE);
    }
    
    // io_statement can start with READ or WRITE
    if (strcmp(sym, "io_statement") == 0) {
        return (token == TOKEN_READ || token == TOKEN_WRITE);
    }
    
    // expression_prime can start with addition_operator (PLUS/MINUS) or be epsilon (FOLLOW set)
    if (strcmp(sym, "expression_prime") == 0) {
        return (token == TOKEN_PLUS || token == TOKEN_MINUS ||
                token == TOKEN_RPAREN || token == TOKEN_SEMICOLON || 
                token == TOKEN_RBRACE || token == TOKEN_COMMA ||
                token == TOKEN_GT || token == TOKEN_LT || token == TOKEN_GE || 
                token == TOKEN_LE || token == TOKEN_EQ || token == TOKEN_NEQ);
    }
    
    // term_prime can start with multiplication_operator (MUL/DIV/MOD) or be epsilon (FOLLOW set)
    if (strcmp(sym, "term_prime") == 0) {
        return (token == TOKEN_MUL || token == TOKEN_DIV || token == TOKEN_MOD ||
                token == TOKEN_PLUS || token == TOKEN_MINUS ||
                token == TOKEN_RPAREN || token == TOKEN_SEMICOLON || 
                token == TOKEN_RBRACE || token == TOKEN_COMMA ||
                token == TOKEN_GT || token == TOKEN_LT || token == TOKEN_GE || 
                token == TOKEN_LE || token == TOKEN_EQ || token == TOKEN_NEQ);
    }
    
    if (strcmp(sym, "if_tail") == 0) {
        // if_tail can start with ELSE, or be epsilon (when ELSE is not present)
        return (token == TOKEN_ELSE || 
                token == TOKEN_STOP || token == TOKEN_RBRACE ||
                token == TOKEN_IDENTIFIER || token == TOKEN_IF || 
                token == TOKEN_WHILE || token == TOKEN_READ || token == TOKEN_WRITE ||
                token == TOKEN_INT || token == TOKEN_FLOAT || 
                token == TOKEN_STRING || token == TOKEN_CHAR);
    }
    
    // Check productions for this non-terminal
    for (int i = 0; i < g->num_productions; i++) {
        if (strcmp(g->productions[i].lhs, sym) == 0) {
            Production* p = &g->productions[i];
            if (p->rhs_len > 0) {
                if (can_start_with(parser, p->rhs[0], token)) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

// Find production for non-terminal and terminal
// This is a simplified LL(1) parser - in a full implementation, we'd use the parsing table
static Production* find_production(LL1Parser* parser, const char* nt, int token) {
    Grammar* g = parser->grammar;
    int nt_idx = get_non_terminal_index(g, nt);
    if (nt_idx == -1) return NULL;
    
    // Special case: if_tail
    if (strcmp(nt, "if_tail") == 0) {
        if (token == TOKEN_ELSE) {
            // Find the production with ELSE
            for (int i = 0; i < g->num_productions; i++) {
                if (strcmp(g->productions[i].lhs, "if_tail") == 0) {
                    if (g->productions[i].rhs_len > 0 && 
                        strcmp(g->productions[i].rhs[0], "ELSE") == 0) {
                        return &g->productions[i];
                    }
                }
            }
        } else {
            // Not ELSE - use epsilon production
            for (int i = 0; i < g->num_productions; i++) {
                if (strcmp(g->productions[i].lhs, "if_tail") == 0) {
                    if (g->productions[i].rhs_len == 1 && 
                        strcmp(g->productions[i].rhs[0], "epsilon") == 0) {
                        return &g->productions[i];
                    }
                }
            }
        }
    }
    
    // Try each production for this non-terminal
    // First pass: try to find a production that can start with the token
    for (int i = 0; i < g->num_productions; i++) {
        if (strcmp(g->productions[i].lhs, nt) != 0) continue;
        
        Production* p = &g->productions[i];
        
        // Check if production can start with this token
        if (p->rhs_len == 0) continue;
        
        // Check first symbol
        const char* first_sym = p->rhs[0];
        
        // Skip epsilon productions for now (handle them separately)
        if (strcmp(first_sym, "epsilon") == 0) continue;
        
        if (can_start_with(parser, first_sym, token)) {
            return p;
        }
    }
    
    // Second pass: check epsilon productions (when token is in FOLLOW set)
    for (int i = 0; i < g->num_productions; i++) {
        if (strcmp(g->productions[i].lhs, nt) != 0) continue;
        
        Production* p = &g->productions[i];
        
        if (p->rhs_len == 1 && strcmp(p->rhs[0], "epsilon") == 0) {
            // Check if token is in FOLLOW(nt)
            // Simplified: for statement_list, epsilon is valid before STOP
            if (strcmp(nt, "statement_list") == 0 && 
                (token == TOKEN_STOP || token == TOKEN_RBRACE)) {
                return p;
            }
            // For expression_prime - epsilon when seeing FOLLOW set tokens
            if (strcmp(nt, "expression_prime") == 0 &&
                (token == TOKEN_RPAREN || token == TOKEN_SEMICOLON || 
                 token == TOKEN_RBRACE || token == TOKEN_COMMA ||
                 token == TOKEN_GT || token == TOKEN_LT || token == TOKEN_GE || 
                 token == TOKEN_LE || token == TOKEN_EQ || token == TOKEN_NEQ)) {
                return p;
            }
            // For term_prime - epsilon when seeing FOLLOW set tokens (includes PLUS/MINUS for higher precedence)
            if (strcmp(nt, "term_prime") == 0 &&
                (token == TOKEN_PLUS || token == TOKEN_MINUS ||
                 token == TOKEN_RPAREN || token == TOKEN_SEMICOLON || 
                 token == TOKEN_RBRACE || token == TOKEN_COMMA ||
                 token == TOKEN_GT || token == TOKEN_LT || token == TOKEN_GE || 
                 token == TOKEN_LE || token == TOKEN_EQ || token == TOKEN_NEQ)) {
                return p;
            }
            // For condition_prime
            if (strcmp(nt, "condition_prime") == 0 &&
                (token == TOKEN_RPAREN || token == TOKEN_RBRACE)) {
                return p;
            }
            
            // For if_tail - epsilon is valid when ELSE is not present
            if (strcmp(nt, "if_tail") == 0 &&
                token != TOKEN_ELSE &&
                (token == TOKEN_STOP || token == TOKEN_RBRACE ||
                 token == TOKEN_IDENTIFIER || token == TOKEN_IF || 
                 token == TOKEN_WHILE || token == TOKEN_READ || token == TOKEN_WRITE ||
                 token == TOKEN_INT || token == TOKEN_FLOAT || 
                 token == TOKEN_STRING || token == TOKEN_CHAR)) {
                return p;
            }
        }
    }
    
    // No valid production found
    fprintf(stderr, "Error: No valid production for %s with token %d\n", nt, token);
    return NULL;
}

// Parse input using LL(1) algorithm
bool parse(LL1Parser* parser, int* input_tokens, int input_len) {
    Grammar* g = parser->grammar;
    ParseStack stack;
    stack.top = -1;
    
    // Create root node
    ParseTreeNode* root = create_node(parser, g->start_symbol, SYM_NON_TERMINAL, -1);
    parser->parse_tree = root;
    stack.nodes[++stack.top] = root;
    
    int input_pos = 0;
    
    while (stack.top >= 0 && input_pos < input_len) {
        ParseTreeNode* top_node = stack.nodes[stack.top];
        int current_token = input_tokens[input_pos];
        
        // Special handling: if we see ELSE when parsing statement_list,
        // it means the previous if_statement should have included ELSE
        // We'll handle this by allowing ELSE to be parsed as part of statement_list
        // and then it will be matched when we get to it in the if_statement
        if (current_token == TOKEN_ELSE && strcmp(top_node->symbol, "statement_list") == 0) {
            // This ELSE belongs to a previous if_statement
            // We need to find where we are and handle it
            // For now, treat ELSE as starting a new statement (which will fail, but shows the issue)
            // Actually, let's check if we can expand statement_list to handle this
            // The issue is that ELSE should be part of if_statement, not statement_list
            // This is a limitation of LL(1) - we'd need LL(2) or more lookahead
            // For now, we'll skip this special case and let the normal parsing handle it
        }
        
        if (is_terminal(top_node->symbol)) {
            // Terminal on stack - match with input
            int expected_token = token_name_to_code(top_node->symbol);
            if (expected_token == current_token) {
                top_node->token_code = current_token;
                stack.top--;  // Pop
                input_pos++;  // Advance input
            } else {
                fprintf(stderr, "Syntax error: expected %s, got token %d\n", 
                        top_node->symbol, current_token);
                return false;
            }
        } else if (strcmp(top_node->symbol, "epsilon") == 0) {
            // Epsilon - just pop
            stack.top--;
        } else {
            // Non-terminal - expand using production
            Production* prod = find_production(parser, top_node->symbol, current_token);
            if (!prod) {
                fprintf(stderr, "Syntax error: no production for %s with token %d\n",
                        top_node->symbol, current_token);
                return false;
            }
            
            // Store production number in the node
            top_node->production_num = prod->prod_num;
            
            // Pop non-terminal
            stack.top--;
            
            // Push RHS symbols in reverse order (for stack)
            // But build tree in forward order
            ParseTreeNode* children[20];
            int num_children = 0;
            
            // First, create all children
            for (int i = 0; i < prod->rhs_len; i++) {
                const char* sym = prod->rhs[i];
                SymbolType sym_type = is_terminal(sym) ? SYM_TERMINAL : SYM_NON_TERMINAL;
                int sym_token = is_terminal(sym) ? token_name_to_code(sym) : -1;
                
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
    if (stack.top < 0 && input_pos >= input_len - 1) {  // -1 because of EOF
        return true;
    }
    
    return false;
}

// Helper function to get node ID or -1 if NULL
static int get_node_id(ParseTreeNode* node) {
    return node ? node->node_id : -1;
}

// Traverse tree and collect all nodes
static void collect_nodes(ParseTreeNode* node, ParseTreeNode** nodes, int* count) {
    if (!node) return;
    
    nodes[(*count)++] = node;
    
    ParseTreeNode* child = node->first_child;
    while (child) {
        collect_nodes(child, nodes, count);
        child = child->next_sibling;
    }
}

// Print parse tree as table with father/sibling relations
void print_parse_tree_table(LL1Parser* parser, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Cannot create output file: %s\n", filename);
        return;
    }
    
    if (!parser->parse_tree) {
        fprintf(f, "No parse tree available\n");
        fclose(f);
        return;
    }
    
    // Collect all nodes
    ParseTreeNode* nodes[1000];
    int node_count = 0;
    collect_nodes(parser->parse_tree, nodes, &node_count);
    
    // Print header
    fprintf(f, "Parse Tree Table (Father-Sibling Representation)\n");
    fprintf(f, "================================================\n\n");
    fprintf(f, "%-10s %-30s %-15s %-15s\n", "Node ID", "Symbol", "Father ID", "Sibling ID");
    fprintf(f, "%-10s %-30s %-15s %-15s\n", 
            "--------", "------------------------------", "---------------", "---------------");
    
    // Print each node
    for (int i = 0; i < node_count; i++) {
        ParseTreeNode* node = nodes[i];
        int father_id = get_node_id(node->parent);
        int sibling_id = get_node_id(node->next_sibling);
        
        fprintf(f, "%-10d %-30s %-15d %-15d\n", 
                node->node_id, 
                node->symbol,
                father_id,
                sibling_id);
    }
    
    fprintf(f, "\nTotal nodes: %d\n", node_count);
    fclose(f);
}

// Helper function to get production by number
static Production* get_production_by_num(Grammar* g, int prod_num) {
    for (int i = 0; i < g->num_productions; i++) {
        if (g->productions[i].prod_num == prod_num) {
            return &g->productions[i];
        }
    }
    return NULL;
}

// Traverse tree in pre-order and collect productions
static void collect_productions(ParseTreeNode* node, Grammar* g, int* prod_list, int* count) {
    if (!node) return;
    
    // If this is a non-terminal that was expanded, add its production
    if (node->type == SYM_NON_TERMINAL && node->production_num > 0) {
        prod_list[(*count)++] = node->production_num;
    }
    
    // Traverse children
    ParseTreeNode* child = node->first_child;
    while (child) {
        collect_productions(child, g, prod_list, count);
        child = child->next_sibling;
    }
}

// Print productions string (2a)
void print_productions_string(LL1Parser* parser, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Cannot create output file: %s\n", filename);
        return;
    }
    
    if (!parser->parse_tree) {
        fprintf(f, "No parse tree available\n");
        fclose(f);
        return;
    }
    
    Grammar* g = parser->grammar;
    
    // Collect all productions used in the parse tree
    int prod_list[1000];
    int prod_count = 0;
    collect_productions(parser->parse_tree, g, prod_list, &prod_count);
    
    // Print each production
    for (int i = 0; i < prod_count; i++) {
        Production* p = get_production_by_num(g, prod_list[i]);
        if (p) {
            fprintf(f, "%d: %s ->", p->prod_num, p->lhs);
            for (int j = 0; j < p->rhs_len; j++) {
                fprintf(f, " %s", p->rhs[j]);
            }
            fprintf(f, "\n");
        }
    }
    
    fprintf(f, "\nTotal productions: %d\n", prod_count);
    fclose(f);
}

// Helper function to remove multiple consecutive spaces
static void normalize_spaces(char* str) {
    char* src = str;
    char* dst = str;
    bool in_space = false;
    
    while (*src) {
        if (*src == ' ') {
            if (!in_space) {
                *dst++ = ' ';
                in_space = true;
            }
        } else {
            *dst++ = *src;
            in_space = false;
        }
        src++;
    }
    *dst = '\0';
}

// Print derivations string (2b) - leftmost derivation
void print_derivations_string(LL1Parser* parser, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Cannot create output file: %s\n", filename);
        return;
    }
    
    if (!parser->parse_tree) {
        fprintf(f, "No parse tree available\n");
        fclose(f);
        return;
    }
    
    Grammar* g = parser->grammar;
    
    fprintf(f, "Leftmost Derivation\n");
    fprintf(f, "===================\n\n");
    
    // Collect all non-terminal nodes in pre-order (leftmost expansion order)
    ParseTreeNode* nt_nodes[1000];
    int nt_count = 0;
    
    // Pre-order traversal to collect non-terminals
    ParseTreeNode* stack[1000];
    int stack_top = 0;
    stack[stack_top++] = parser->parse_tree;
    
    while (stack_top > 0) {
        ParseTreeNode* node = stack[--stack_top];
        
        if (node->type == SYM_NON_TERMINAL && node->production_num > 0) {
            nt_nodes[nt_count++] = node;
        }
        
        // Push children in reverse order (so leftmost is processed first)
        ParseTreeNode* child = node->first_child;
        ParseTreeNode* children[20];
        int child_count = 0;
        while (child) {
            children[child_count++] = child;
            child = child->next_sibling;
        }
        for (int i = child_count - 1; i >= 0; i--) {
            stack[stack_top++] = children[i];
        }
    }
    
    // Build derivation step by step
    char current_derivation[5000];
    snprintf(current_derivation, sizeof(current_derivation), "%s", parser->parse_tree->symbol);
    fprintf(f, "Step 0: %s\n", current_derivation);
    
    int step = 1;
    for (int i = 0; i < nt_count; i++) {
        ParseTreeNode* node = nt_nodes[i];
        Production* p = get_production_by_num(g, node->production_num);
        
        if (!p) continue;
        
        // Build replacement string from children
        char replacement[5000] = "";
        bool first = true;
        ParseTreeNode* child = node->first_child;
        while (child) {
            if (strcmp(child->symbol, "epsilon") != 0) {
                if (!first) {
                    strcat(replacement, " ");
                }
                strcat(replacement, child->symbol);
                first = false;
            }
            child = child->next_sibling;
        }
        
        // Replace leftmost occurrence of node->symbol
        char* pos = strstr(current_derivation, node->symbol);
        if (pos) {
            char new_derivation[5000];
            int prefix_len = pos - current_derivation;
            strncpy(new_derivation, current_derivation, prefix_len);
            new_derivation[prefix_len] = '\0';
            
            // Add space before replacement if needed
            if (prefix_len > 0 && new_derivation[prefix_len - 1] != ' ' && replacement[0] != '\0') {
                strcat(new_derivation, " ");
            }
            strcat(new_derivation, replacement);
            
            // Add space after replacement if needed
            int after_len = strlen(node->symbol);
            if (replacement[0] != '\0' && pos[after_len] != ' ' && pos[after_len] != '\0') {
                strcat(new_derivation, " ");
            }
            strcat(new_derivation, pos + after_len);
            
            strcpy(current_derivation, new_derivation);
            normalize_spaces(current_derivation);
            
            fprintf(f, "Step %d: %s\n", step++, current_derivation);
        }
    }
    
    fprintf(f, "\nTotal derivation steps: %d\n", step);
    fclose(f);
}

// Free parser
void free_parser(LL1Parser* parser) {
    if (!parser) return;
    
    if (parser->parsing_table) {
        for (int i = 0; i < parser->grammar->num_non_terminals; i++) {
            free(parser->parsing_table[i]);
        }
        free(parser->parsing_table);
    }
    
    for (int i = 0; i < parser->grammar->num_non_terminals; i++) {
        free(parser->first_sets[i]);
        free(parser->follow_sets[i]);
    }
    
    free(parser);
}

// Free grammar
void free_grammar(Grammar* g) {
    free(g);
}

