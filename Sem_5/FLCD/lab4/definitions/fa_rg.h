#ifndef FA_RG_H
#define FA_RG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STATES 100
#define MAX_SYMBOLS 100
#define MAX_PRODUCTIONS 500
#define MAX_SYMBOL_LEN 50
#define MAX_PRODUCTION_RHS 200

// Transition: (state, symbol) -> next_state
typedef struct
{
    char state[MAX_SYMBOL_LEN];
    char symbol[MAX_SYMBOL_LEN];
    char next_state[MAX_SYMBOL_LEN];
} FA_Transition;

typedef struct
{
    char states[MAX_STATES][MAX_SYMBOL_LEN];
    int num_states;
    char alphabet[MAX_SYMBOLS][MAX_SYMBOL_LEN];
    int num_alphabet;
    FA_Transition transitions[MAX_PRODUCTIONS];
    int num_transitions;
    char start_state[MAX_SYMBOL_LEN];
    char final_states[MAX_STATES][MAX_SYMBOL_LEN];
    int num_final_states;
} FiniteAutomata;

typedef struct
{
    char non_terminal[MAX_SYMBOL_LEN];
    char productions[MAX_PRODUCTIONS][MAX_PRODUCTION_RHS];
    int num_productions;
} RG_NonTerminal;

typedef struct
{
    char non_terminals[MAX_STATES][MAX_SYMBOL_LEN];
    int num_non_terminals;
    char terminals[MAX_SYMBOLS][MAX_SYMBOL_LEN];
    int num_terminals;
    RG_NonTerminal productions[MAX_STATES];
    int num_production_rules;
    char start_symbol[MAX_SYMBOL_LEN];
} RegularGrammar;

FiniteAutomata *fa_create(void);
void fa_add_state(FiniteAutomata *fa, const char *state);
void fa_add_alphabet_symbol(FiniteAutomata *fa, const char *symbol);
void fa_add_transition(FiniteAutomata *fa, const char *from_state, const char *symbol, const char *to_state);
void fa_set_start_state(FiniteAutomata *fa, const char *state);
void fa_add_final_state(FiniteAutomata *fa, const char *state);
void fa_print(FiniteAutomata *fa);
void fa_print_file(FiniteAutomata *fa, FILE *file);
void fa_free(FiniteAutomata *fa);

RegularGrammar *rg_create(void);
void rg_add_non_terminal(RegularGrammar *rg, const char *symbol);
void rg_add_terminal(RegularGrammar *rg, const char *symbol);
void rg_add_production(RegularGrammar *rg, const char *non_terminal, const char *rhs);
void rg_set_start_symbol(RegularGrammar *rg, const char *symbol);
void rg_print(RegularGrammar *rg);
void rg_print_file(RegularGrammar *rg, FILE *file);
void rg_free(RegularGrammar *rg);

RegularGrammar *fa_to_rg(FiniteAutomata *fa);
FiniteAutomata *rg_to_fa(RegularGrammar *rg);

bool fa_verify(FiniteAutomata *fa, const char *input);
bool rg_verify(RegularGrammar *rg, const char *input);

const char *map_char_to_symbol(char c);

#endif