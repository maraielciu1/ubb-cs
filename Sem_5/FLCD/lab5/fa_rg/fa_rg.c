#include "fa_rg.h"

FiniteAutomata *fa_create(void)
{
    FiniteAutomata *fa = (FiniteAutomata *)calloc(1, sizeof(FiniteAutomata));
    return fa;
}

void fa_add_state(FiniteAutomata *fa, const char *state)
{
    // Check if state already exists
    for (int i = 0; i < fa->num_states; i++)
    {
        if (strcmp(fa->states[i], state) == 0)
        {
            return;
        }
    }
    if (fa->num_states < MAX_STATES)
    {
        strcpy(fa->states[fa->num_states], state);
        fa->num_states++;
    }
}

void fa_add_alphabet_symbol(FiniteAutomata *fa, const char *symbol)
{
    // Check if symbol already exists
    for (int i = 0; i < fa->num_alphabet; i++)
    {
        if (strcmp(fa->alphabet[i], symbol) == 0)
        {
            return;
        }
    }
    if (fa->num_alphabet < MAX_SYMBOLS)
    {
        strcpy(fa->alphabet[fa->num_alphabet], symbol);
        fa->num_alphabet++;
    }
}

void fa_add_transition(FiniteAutomata *fa, const char *from_state, const char *symbol, const char *to_state)
{
    if (fa->num_transitions < MAX_PRODUCTIONS)
    {
        strcpy(fa->transitions[fa->num_transitions].state, from_state);
        strcpy(fa->transitions[fa->num_transitions].symbol, symbol);
        strcpy(fa->transitions[fa->num_transitions].next_state, to_state);
        fa->num_transitions++;

        // Ensure states are in the states set
        fa_add_state(fa, from_state);
        fa_add_state(fa, to_state);

        // Ensure symbol is in alphabet
        fa_add_alphabet_symbol(fa, symbol);
    }
}

void fa_set_start_state(FiniteAutomata *fa, const char *state)
{
    strcpy(fa->start_state, state);
    fa_add_state(fa, state);
}

void fa_add_final_state(FiniteAutomata *fa, const char *state)
{
    // Check if already final
    for (int i = 0; i < fa->num_final_states; i++)
    {
        if (strcmp(fa->final_states[i], state) == 0)
        {
            return;
        }
    }
    if (fa->num_final_states < MAX_STATES)
    {
        strcpy(fa->final_states[fa->num_final_states], state);
        fa->num_final_states++;
        fa_add_state(fa, state);
    }
}

void fa_print(FiniteAutomata *fa)
{
    fa_print_file(fa, stdout);
}

void fa_print_file(FiniteAutomata *fa, FILE *file)
{
    fprintf(file, "Finite Automaton:\n");
    fprintf(file, "  States: {");
    for (int i = 0; i < fa->num_states; i++)
    {
        fprintf(file, "%s", fa->states[i]);
        if (i < fa->num_states - 1)
            fprintf(file, ", ");
    }
    fprintf(file, "}\n");

    fprintf(file, "  Alphabet: {");
    for (int i = 0; i < fa->num_alphabet; i++)
    {
        fprintf(file, "%s", fa->alphabet[i]);
        if (i < fa->num_alphabet - 1)
            fprintf(file, ", ");
    }
    fprintf(file, "}\n");

    fprintf(file, "  Start State: %s\n", fa->start_state);

    fprintf(file, "  Final States: {");
    for (int i = 0; i < fa->num_final_states; i++)
    {
        fprintf(file, "%s", fa->final_states[i]);
        if (i < fa->num_final_states - 1)
            fprintf(file, ", ");
    }
    fprintf(file, "}\n");

    fprintf(file, "  Transitions:\n");
    for (int i = 0; i < fa->num_transitions; i++)
    {
        fprintf(file, "    δ(%s, %s) = %s\n",
                fa->transitions[i].state,
                fa->transitions[i].symbol,
                fa->transitions[i].next_state);
    }
}

void fa_free(FiniteAutomata *fa)
{
    free(fa);
}

/* ==================== RG Implementation ==================== */

RegularGrammar *rg_create(void)
{
    RegularGrammar *rg = (RegularGrammar *)calloc(1, sizeof(RegularGrammar));
    return rg;
}

void rg_add_non_terminal(RegularGrammar *rg, const char *symbol)
{
    // Check if already exists
    for (int i = 0; i < rg->num_non_terminals; i++)
    {
        if (strcmp(rg->non_terminals[i], symbol) == 0)
        {
            return;
        }
    }
    if (rg->num_non_terminals < MAX_STATES)
    {
        strcpy(rg->non_terminals[rg->num_non_terminals], symbol);
        rg->num_non_terminals++;
    }
}

void rg_add_terminal(RegularGrammar *rg, const char *symbol)
{
    // Check if already exists
    for (int i = 0; i < rg->num_terminals; i++)
    {
        if (strcmp(rg->terminals[i], symbol) == 0)
        {
            return;
        }
    }
    if (rg->num_terminals < MAX_SYMBOLS)
    {
        strcpy(rg->terminals[rg->num_terminals], symbol);
        rg->num_terminals++;
    }
}

void rg_add_production(RegularGrammar *rg, const char *non_terminal, const char *rhs)
{
    // Find or create non-terminal entry
    int nt_index = -1;
    for (int i = 0; i < rg->num_production_rules; i++)
    {
        if (strcmp(rg->productions[i].non_terminal, non_terminal) == 0)
        {
            nt_index = i;
            break;
        }
    }

    if (nt_index == -1)
    {
        // Create new non-terminal entry
        if (rg->num_production_rules < MAX_STATES)
        {
            nt_index = rg->num_production_rules;
            strcpy(rg->productions[nt_index].non_terminal, non_terminal);
            rg->productions[nt_index].num_productions = 0;
            rg->num_production_rules++;
            rg_add_non_terminal(rg, non_terminal);
        }
        else
        {
            return; // Cannot add more
        }
    }

    // Add production to this non-terminal
    if (rg->productions[nt_index].num_productions < MAX_PRODUCTIONS)
    {
        strcpy(rg->productions[nt_index].productions[rg->productions[nt_index].num_productions], rhs);
        rg->productions[nt_index].num_productions++;
    }
}

void rg_set_start_symbol(RegularGrammar *rg, const char *symbol)
{
    strcpy(rg->start_symbol, symbol);
    rg_add_non_terminal(rg, symbol);
}

void rg_print(RegularGrammar *rg)
{
    rg_print_file(rg, stdout);
}

void rg_print_file(RegularGrammar *rg, FILE *file)
{
    fprintf(file, "Regular Grammar (Right Linear):\n");

    fprintf(file, "  Non-terminals: {");
    for (int i = 0; i < rg->num_non_terminals; i++)
    {
        fprintf(file, "%s", rg->non_terminals[i]);
        if (i < rg->num_non_terminals - 1)
            fprintf(file, ", ");
    }
    fprintf(file, "}\n");

    fprintf(file, "  Terminals: {");
    for (int i = 0; i < rg->num_terminals; i++)
    {
        fprintf(file, "%s", rg->terminals[i]);
        if (i < rg->num_terminals - 1)
            fprintf(file, ", ");
    }
    fprintf(file, "}\n");

    fprintf(file, "  Start Symbol: %s\n", rg->start_symbol);

    fprintf(file, "  Production Rules:\n");
    for (int i = 0; i < rg->num_production_rules; i++)
    {
        for (int j = 0; j < rg->productions[i].num_productions; j++)
        {
            fprintf(file, "    %s → %s\n",
                    rg->productions[i].non_terminal,
                    rg->productions[i].productions[j]);
        }
    }
}

void rg_free(RegularGrammar *rg)
{
    free(rg);
}

// Helper function to map state to non-terminal name
static void state_to_nonterminal(const char *state, char *non_terminal)
{
    if (strcmp(state, "q0") == 0)
    {
        strcpy(non_terminal, "S");
    }
    else if (strcmp(state, "q1") == 0)
    {
        strcpy(non_terminal, "A");
    }
    else if (strcmp(state, "q2") == 0)
    {
        strcpy(non_terminal, "B");
    }
    else if (strcmp(state, "q3") == 0)
    {
        strcpy(non_terminal, "C");
    }
    else if (strcmp(state, "q4") == 0)
    {
        strcpy(non_terminal, "D");
    }
    else if (strcmp(state, "q5") == 0)
    {
        strcpy(non_terminal, "E");
    }
    else
    {
        // Convert to uppercase
        strcpy(non_terminal, state);
        for (int i = 0; non_terminal[i]; i++)
        {
            if (non_terminal[i] >= 'a' && non_terminal[i] <= 'z')
            {
                non_terminal[i] = non_terminal[i] - 'a' + 'A';
            }
        }
    }
}

RegularGrammar *fa_to_rg(FiniteAutomata *fa)
{
    RegularGrammar *rg = rg_create();

    // Map all states to non-terminals
    char state_map[MAX_STATES][MAX_SYMBOL_LEN];
    char non_term_map[MAX_STATES][MAX_SYMBOL_LEN];

    for (int i = 0; i < fa->num_states; i++)
    {
        strcpy(state_map[i], fa->states[i]);
        state_to_nonterminal(fa->states[i], non_term_map[i]);
        rg_add_non_terminal(rg, non_term_map[i]);
    }

    // Copy terminals from alphabet
    for (int i = 0; i < fa->num_alphabet; i++)
    {
        rg_add_terminal(rg, fa->alphabet[i]);
    }

    // Set start symbol
    char start_nt[MAX_SYMBOL_LEN];
    for (int i = 0; i < fa->num_states; i++)
    {
        if (strcmp(fa->states[i], fa->start_state) == 0)
        {
            state_to_nonterminal(fa->states[i], start_nt);
            rg_set_start_symbol(rg, start_nt);
            break;
        }
    }

    // Convert transitions to productions
    for (int i = 0; i < fa->num_transitions; i++)
    {
        // Find non-terminal for from_state
        char from_nt[MAX_SYMBOL_LEN];
        for (int j = 0; j < fa->num_states; j++)
        {
            if (strcmp(fa->states[j], fa->transitions[i].state) == 0)
            {
                state_to_nonterminal(fa->states[j], from_nt);
                break;
            }
        }

        // Find non-terminal for to_state
        char to_nt[MAX_SYMBOL_LEN];
        bool is_final = false;
        for (int j = 0; j < fa->num_states; j++)
        {
            if (strcmp(fa->states[j], fa->transitions[i].next_state) == 0)
            {
                state_to_nonterminal(fa->states[j], to_nt);
                // Check if it's a final state
                for (int k = 0; k < fa->num_final_states; k++)
                {
                    if (strcmp(fa->final_states[k], fa->transitions[i].next_state) == 0)
                    {
                        is_final = true;
                        break;
                    }
                }
                break;
            }
        }

        // Create production: symbol non_terminal or just symbol if final
        char production[MAX_PRODUCTION_RHS];
        if (is_final)
        {
            strcpy(production, fa->transitions[i].symbol);
        }
        else
        {
            snprintf(production, MAX_PRODUCTION_RHS, "%s %s",
                     fa->transitions[i].symbol, to_nt);
        }
        rg_add_production(rg, from_nt, production);
    }

    // Add epsilon productions for final states
    for (int i = 0; i < fa->num_final_states; i++)
    {
        char final_nt[MAX_SYMBOL_LEN];
        for (int j = 0; j < fa->num_states; j++)
        {
            if (strcmp(fa->states[j], fa->final_states[i]) == 0)
            {
                state_to_nonterminal(fa->states[j], final_nt);
                // Check if this non-terminal already has epsilon
                bool has_epsilon = false;
                for (int k = 0; k < rg->num_production_rules; k++)
                {
                    if (strcmp(rg->productions[k].non_terminal, final_nt) == 0)
                    {
                        for (int l = 0; l < rg->productions[k].num_productions; l++)
                        {
                            if (strcmp(rg->productions[k].productions[l], "epsilon") == 0 ||
                                strcmp(rg->productions[k].productions[l], "epsilon") == 0)
                            {
                                has_epsilon = true;
                                break;
                            }
                        }
                        break;
                    }
                }
                if (!has_epsilon)
                {
                    rg_add_production(rg, final_nt, "epsilon");
                }
                break;
            }
        }
    }

    return rg;
}

FiniteAutomata *rg_to_fa(RegularGrammar *rg)
{
    FiniteAutomata *fa = fa_create();

    // Map non-terminals to states (q0, q1, q2, ...)
    char nt_to_state[MAX_STATES][MAX_SYMBOL_LEN];
    for (int i = 0; i < rg->num_non_terminals; i++)
    {
        snprintf(nt_to_state[i], MAX_SYMBOL_LEN, "q%d", i);
        fa_add_state(fa, nt_to_state[i]);
    }

    // Add a final state if needed
    char final_state[MAX_SYMBOL_LEN];
    snprintf(final_state, MAX_SYMBOL_LEN, "q%d", rg->num_non_terminals);
    fa_add_state(fa, final_state);
    fa_add_final_state(fa, final_state);

    // Copy terminals to alphabet
    for (int i = 0; i < rg->num_terminals; i++)
    {
        fa_add_alphabet_symbol(fa, rg->terminals[i]);
    }

    // Set start state
    for (int i = 0; i < rg->num_non_terminals; i++)
    {
        if (strcmp(rg->non_terminals[i], rg->start_symbol) == 0)
        {
            fa_set_start_state(fa, nt_to_state[i]);
            break;
        }
    }

    // Convert productions to transitions
    for (int i = 0; i < rg->num_production_rules; i++)
    {
        const char *from_nt = rg->productions[i].non_terminal;
        int from_state_idx = -1;
        for (int j = 0; j < rg->num_non_terminals; j++)
        {
            if (strcmp(rg->non_terminals[j], from_nt) == 0)
            {
                from_state_idx = j;
                break;
            }
        }
        if (from_state_idx == -1)
            continue;

        for (int j = 0; j < rg->productions[i].num_productions; j++)
        {
            const char *rhs = rg->productions[i].productions[j];

            // Handle epsilon
            if (strcmp(rhs, "epsilon") == 0 || strcmp(rhs, "epsilon") == 0)
            {
                fa_add_final_state(fa, nt_to_state[from_state_idx]);
                continue;
            }

            // Parse production: "symbol non_terminal" or just "symbol"
            char symbol[MAX_SYMBOL_LEN] = {0};
            char next_nt[MAX_SYMBOL_LEN] = {0};

            // Try to split by space
            int space_pos = -1;
            for (int k = 0; rhs[k] && rhs[k] != '\0'; k++)
            {
                if (rhs[k] == ' ')
                {
                    space_pos = k;
                    break;
                }
            }

            if (space_pos > 0)
            {
                // Has both symbol and non-terminal
                strncpy(symbol, rhs, space_pos);
                symbol[space_pos] = '\0';
                strcpy(next_nt, rhs + space_pos + 1);
            }
            else
            {
                // Just symbol (terminal production)
                strcpy(symbol, rhs);
            }

            // Find next state
            char to_state[MAX_SYMBOL_LEN];
            if (strlen(next_nt) > 0)
            {
                // Find non-terminal
                int to_nt_idx = -1;
                for (int k = 0; k < rg->num_non_terminals; k++)
                {
                    if (strcmp(rg->non_terminals[k], next_nt) == 0)
                    {
                        to_nt_idx = k;
                        break;
                    }
                }
                if (to_nt_idx >= 0)
                {
                    strcpy(to_state, nt_to_state[to_nt_idx]);
                }
                else
                {
                    strcpy(to_state, final_state);
                }
            }
            else
            {
                // Terminal production -> final state
                strcpy(to_state, final_state);
            }

            fa_add_transition(fa, nt_to_state[from_state_idx], symbol, to_state);
        }
    }

    return fa;
}

/* ==================== Verify Functions ==================== */

// Helper function to map actual character to abstract symbol
const char *map_char_to_symbol(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    {
        return "letter";
    }
    else if (c >= '0' && c <= '9')
    {
        return "digit"; // All digits (including 0)
    }
    else if (c == '_')
    {
        return "_";
    }
    else if (c == ' ')
    {
        return "space";
    }
    else if (c == '.')
    {
        return ".";
    }
    else if (c == ';')
    {
        return ";";
    }
    else if (c == '!')
    {
        return "!";
    }
    else if (c == '\'')
    {
        return "'";
    }
    else if (c == '"')
    {
        return "\"";
    }
    else if (c == '+' || c == '-')
    {
        return (c == '+') ? "+" : "-";
    }
    // Return as single character string for other cases
    static char char_str[2];
    char_str[0] = c;
    char_str[1] = '\0';
    return char_str;
}

bool fa_verify(FiniteAutomata *fa, const char *input)
{
    if (fa == NULL || input == NULL)
    {
        return false;
    }

    char current_state[MAX_SYMBOL_LEN];
    strcpy(current_state, fa->start_state);

    int len = strlen(input);
    for (int i = 0; i < len; i++)
    {
        const char *symbol = map_char_to_symbol(input[i]);
        bool found_transition = false;

        // Check if this is an integer constant pattern
        bool has_decimal = false;
        for (int k = 0; k < len; k++)
        {
            if (input[k] == '.')
            {
                has_decimal = true;
                break;
            }
        }

        if (!has_decimal && input[i] == '0' &&
            (i == 0 || (i == 1 && (input[0] == '+' || input[0] == '-'))))
        {
            symbol = "0";
        }

        // Look for a transition from current_state with this symbol
        for (int j = 0; j < fa->num_transitions; j++)
        {
            if (strcmp(fa->transitions[j].state, current_state) == 0 &&
                strcmp(fa->transitions[j].symbol, symbol) == 0)
            {
                strcpy(current_state, fa->transitions[j].next_state);
                found_transition = true;
                break;
            }
        }

        if (!found_transition)
        {
            return false;
        }

        // Special check: if we're in q3 (final state for "0" in integer FA) and there are more characters, reject
        // Only apply this check for integer constants (check if this is an integer pattern)
        bool has_decimal_check = false;
        for (int k = 0; k < len; k++)
        {
            if (input[k] == '.')
            {
                has_decimal_check = true;
                break;
            }
        }
        // Only reject q3 with more chars if this is NOT a float (no decimal point)
        if (!has_decimal_check && strcmp(current_state, "q3") == 0 && i < len - 1)
        {
            return false; // "0" followed by more digits is invalid
        }
    }

    for (int i = 0; i < fa->num_final_states; i++)
    {
        if (strcmp(fa->final_states[i], current_state) == 0)
        {
            return true;
        }
    }

    return false;
}

bool rg_verify(RegularGrammar *rg, const char *input)
{
    if (rg == NULL || input == NULL)
    {
        return false;
    }

    // Convert RG to FA and verify using FA
    FiniteAutomata *fa = rg_to_fa(rg);
    bool result = fa_verify(fa, input);
    fa_free(fa);
    return result;
}

/* ==================== FA Creation Functions for Identifiers and Constants ==================== */

FiniteAutomata *create_identifier_fa(void)
{
    FiniteAutomata *fa = fa_create();

    fa_add_state(fa, "q0");
    fa_add_state(fa, "q1");
    fa_add_state(fa, "q2");

    fa_add_alphabet_symbol(fa, "!");
    fa_add_alphabet_symbol(fa, "letter");
    fa_add_alphabet_symbol(fa, "digit");
    fa_add_alphabet_symbol(fa, "_");

    fa_set_start_state(fa, "q0");
    fa_add_final_state(fa, "q2");

    fa_add_transition(fa, "q0", "!", "q1");
    fa_add_transition(fa, "q1", "letter", "q2");
    fa_add_transition(fa, "q2", "letter", "q2");
    fa_add_transition(fa, "q2", "digit", "q2");
    fa_add_transition(fa, "q2", "_", "q2");

    return fa;
}

FiniteAutomata *create_int_constant_fa(void)
{
    FiniteAutomata *fa = fa_create();

    fa_add_state(fa, "q0");
    fa_add_state(fa, "q1");
    fa_add_state(fa, "q2");
    fa_add_state(fa, "q3");

    fa_add_alphabet_symbol(fa, "+");
    fa_add_alphabet_symbol(fa, "-");
    fa_add_alphabet_symbol(fa, "0");
    fa_add_alphabet_symbol(fa, "digit");

    fa_set_start_state(fa, "q0");
    fa_add_final_state(fa, "q2");
    fa_add_final_state(fa, "q3");

    fa_add_transition(fa, "q0", "0", "q3");
    fa_add_transition(fa, "q0", "+", "q1");
    fa_add_transition(fa, "q0", "-", "q1");
    fa_add_transition(fa, "q1", "digit", "q2");
    fa_add_transition(fa, "q0", "digit", "q2");
    fa_add_transition(fa, "q2", "digit", "q2");
    fa_add_transition(fa, "q2", "0", "q2");

    return fa;
}

FiniteAutomata *create_float_constant_fa(void)
{
    FiniteAutomata *fa = fa_create();

    fa_add_state(fa, "q0");
    fa_add_state(fa, "q1");
    fa_add_state(fa, "q2");
    fa_add_state(fa, "q3");
    fa_add_state(fa, "q4");

    fa_add_alphabet_symbol(fa, "+");
    fa_add_alphabet_symbol(fa, "-");
    fa_add_alphabet_symbol(fa, "digit");
    fa_add_alphabet_symbol(fa, ".");

    fa_set_start_state(fa, "q0");
    fa_add_final_state(fa, "q4");

    fa_add_transition(fa, "q0", "+", "q1");
    fa_add_transition(fa, "q0", "-", "q1");
    fa_add_transition(fa, "q0", "digit", "q2");
    fa_add_transition(fa, "q1", "digit", "q2");
    fa_add_transition(fa, "q2", "digit", "q2");
    fa_add_transition(fa, "q2", ".", "q3");
    fa_add_transition(fa, "q3", "digit", "q4");
    fa_add_transition(fa, "q4", "digit", "q4");

    return fa;
}

FiniteAutomata *create_char_constant_fa(void)
{
    FiniteAutomata *fa = fa_create();

    fa_add_state(fa, "q0");
    fa_add_state(fa, "q1");
    fa_add_state(fa, "q2");
    fa_add_state(fa, "q3");

    fa_add_alphabet_symbol(fa, "'");
    fa_add_alphabet_symbol(fa, "letter");
    fa_add_alphabet_symbol(fa, "digit");
    fa_add_alphabet_symbol(fa, "_");

    fa_set_start_state(fa, "q0");
    fa_add_final_state(fa, "q3");

    fa_add_transition(fa, "q0", "'", "q1");
    fa_add_transition(fa, "q1", "letter", "q2");
    fa_add_transition(fa, "q1", "digit", "q2");
    fa_add_transition(fa, "q1", "_", "q2");
    fa_add_transition(fa, "q2", "'", "q3");

    return fa;
}

FiniteAutomata *create_string_constant_fa(void)
{
    FiniteAutomata *fa = fa_create();

    fa_add_state(fa, "q0");
    fa_add_state(fa, "q1");
    fa_add_state(fa, "q2");
    fa_add_state(fa, "q3");

    fa_add_alphabet_symbol(fa, "\"");
    fa_add_alphabet_symbol(fa, "letter");
    fa_add_alphabet_symbol(fa, "digit");
    fa_add_alphabet_symbol(fa, "_");
    fa_add_alphabet_symbol(fa, "space");
    fa_add_alphabet_symbol(fa, ".");
    fa_add_alphabet_symbol(fa, ";");
    fa_add_alphabet_symbol(fa, "!");

    fa_set_start_state(fa, "q0");
    fa_add_final_state(fa, "q3");

    fa_add_transition(fa, "q0", "\"", "q1");
    fa_add_transition(fa, "q1", "\"", "q3");
    fa_add_transition(fa, "q1", "letter", "q2");
    fa_add_transition(fa, "q1", "digit", "q2");
    fa_add_transition(fa, "q1", "_", "q2");
    fa_add_transition(fa, "q1", "space", "q2");
    fa_add_transition(fa, "q1", ".", "q2");
    fa_add_transition(fa, "q1", ";", "q2");
    fa_add_transition(fa, "q1", "!", "q2");
    fa_add_transition(fa, "q2", "letter", "q2");
    fa_add_transition(fa, "q2", "digit", "q2");
    fa_add_transition(fa, "q2", "_", "q2");
    fa_add_transition(fa, "q2", "space", "q2");
    fa_add_transition(fa, "q2", ".", "q2");
    fa_add_transition(fa, "q2", ";", "q2");
    fa_add_transition(fa, "q2", "!", "q2");
    fa_add_transition(fa, "q2", "\"", "q3");

    return fa;
}