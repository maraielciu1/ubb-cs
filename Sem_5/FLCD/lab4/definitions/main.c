#include <stdio.h>
#include <stdlib.h>
#include "fa_rg.h"

// Create FA for identifiers: !letter (letter|digit|_)*
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

// Create RG for identifiers
RegularGrammar *create_identifier_rg(void)
{
    RegularGrammar *rg = rg_create();

    rg_add_non_terminal(rg, "IDENTIFIER");
    rg_add_non_terminal(rg, "ID_CONT");

    rg_add_terminal(rg, "!");
    rg_add_terminal(rg, "letter");
    rg_add_terminal(rg, "digit");
    rg_add_terminal(rg, "_");

    rg_set_start_symbol(rg, "IDENTIFIER");

    rg_add_production(rg, "IDENTIFIER", "! ID_CONT");
    rg_add_production(rg, "ID_CONT", "letter");
    rg_add_production(rg, "ID_CONT", "letter ID_CONT");
    rg_add_production(rg, "ID_CONT", "digit ID_CONT");
    rg_add_production(rg, "ID_CONT", "_ ID_CONT");

    return rg;
}

// Create FA for integer constants: 0 | [+-]?[1-9][0-9]*
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
    // Only nonzero digits (1-9) can start number after sign or directly
    // We'll handle this in verify by checking the actual character
    fa_add_transition(fa, "q1", "digit", "q2");
    fa_add_transition(fa, "q0", "digit", "q2");
    fa_add_transition(fa, "q2", "digit", "q2");
    fa_add_transition(fa, "q2", "0", "q2"); // Allow 0 in the digit sequence

    return fa;
}

// Create RG for integer constants
RegularGrammar *create_int_constant_rg(void)
{
    RegularGrammar *rg = rg_create();

    rg_add_non_terminal(rg, "INT_CONST");
    rg_add_non_terminal(rg, "UNSIGNED_INT");
    rg_add_non_terminal(rg, "DIGIT_SEQ");

    rg_add_terminal(rg, "0");
    rg_add_terminal(rg, "+");
    rg_add_terminal(rg, "-");
    rg_add_terminal(rg, "digit");

    rg_set_start_symbol(rg, "INT_CONST");

    rg_add_production(rg, "INT_CONST", "0");
    rg_add_production(rg, "INT_CONST", "+ UNSIGNED_INT");
    rg_add_production(rg, "INT_CONST", "- UNSIGNED_INT");
    rg_add_production(rg, "INT_CONST", "UNSIGNED_INT");
    rg_add_production(rg, "UNSIGNED_INT", "digit");
    rg_add_production(rg, "UNSIGNED_INT", "digit DIGIT_SEQ");
    rg_add_production(rg, "DIGIT_SEQ", "digit");
    rg_add_production(rg, "DIGIT_SEQ", "digit DIGIT_SEQ");

    return rg;
}

// Create FA for float constants: [+-]?[0-9]+\.[0-9]+
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

// Create RG for float constants
RegularGrammar *create_float_constant_rg(void)
{
    RegularGrammar *rg = rg_create();

    rg_add_non_terminal(rg, "FLOAT_CONST");
    rg_add_non_terminal(rg, "INT_PART");
    rg_add_non_terminal(rg, "DEC_PART");

    rg_add_terminal(rg, "+");
    rg_add_terminal(rg, "-");
    rg_add_terminal(rg, "digit");
    rg_add_terminal(rg, ".");

    rg_set_start_symbol(rg, "FLOAT_CONST");

    rg_add_production(rg, "FLOAT_CONST", "INT_PART . DEC_PART");
    rg_add_production(rg, "FLOAT_CONST", "+ INT_PART . DEC_PART");
    rg_add_production(rg, "FLOAT_CONST", "- INT_PART . DEC_PART");
    rg_add_production(rg, "INT_PART", "digit");
    rg_add_production(rg, "INT_PART", "digit INT_PART");
    rg_add_production(rg, "DEC_PART", "digit");
    rg_add_production(rg, "DEC_PART", "digit DEC_PART");

    return rg;
}

// Create FA for character constants: 'letter' | 'digit' | '_'
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

// Create RG for character constants
RegularGrammar *create_char_constant_rg(void)
{
    RegularGrammar *rg = rg_create();

    rg_add_non_terminal(rg, "CHAR_CONST");

    rg_add_terminal(rg, "'");
    rg_add_terminal(rg, "letter");
    rg_add_terminal(rg, "digit");
    rg_add_terminal(rg, "_");

    rg_set_start_symbol(rg, "CHAR_CONST");

    rg_add_production(rg, "CHAR_CONST", "' letter '");
    rg_add_production(rg, "CHAR_CONST", "' digit '");
    rg_add_production(rg, "CHAR_CONST", "' _ '");

    return rg;
}

// Create FA for string constants: " [letter|digit|_|space|.;!]* "
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
    fa_add_transition(fa, "q1", "\"", "q3"); // Empty string
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

// Create RG for string constants
RegularGrammar *create_string_constant_rg(void)
{
    RegularGrammar *rg = rg_create();

    rg_add_non_terminal(rg, "STRING_CONST");
    rg_add_non_terminal(rg, "CONTENT");

    rg_add_terminal(rg, "\"");
    rg_add_terminal(rg, "letter");
    rg_add_terminal(rg, "digit");
    rg_add_terminal(rg, "_");
    rg_add_terminal(rg, "space");
    rg_add_terminal(rg, ".");
    rg_add_terminal(rg, ";");
    rg_add_terminal(rg, "!");

    rg_set_start_symbol(rg, "STRING_CONST");

    rg_add_production(rg, "STRING_CONST", "\" CONTENT \"");
    rg_add_production(rg, "STRING_CONST", "\" \"");
    rg_add_production(rg, "CONTENT", "epsilon");
    rg_add_production(rg, "CONTENT", "letter CONTENT");
    rg_add_production(rg, "CONTENT", "digit CONTENT");
    rg_add_production(rg, "CONTENT", "_ CONTENT");
    rg_add_production(rg, "CONTENT", "space CONTENT");
    rg_add_production(rg, "CONTENT", ". CONTENT");
    rg_add_production(rg, "CONTENT", "; CONTENT");
    rg_add_production(rg, "CONTENT", "! CONTENT");

    return rg;
}

int main(void)
{
    FILE *output = fopen("output.txt", "w");
    if (output == NULL)
    {
        printf("ERROR: Cannot create output.txt\n");
        return 1;
    }
    fprintf(output, "FA and RG for Identifiers and Constants\n");

    FiniteAutomata *test_fa = fa_create();
    if (test_fa == NULL)
    {
        fprintf(output, "ERROR: Failed to create FA\n");
        fclose(output);
        return 1;
    }
    fprintf(output, "FA structure created successfully\n");
    fa_free(test_fa);

    RegularGrammar *test_rg = rg_create();
    if (test_rg == NULL)
    {
        fprintf(output, "ERROR: Failed to create RG\n");
        fclose(output);
        return 1;
    }
    fprintf(output, "RG structure created successfully\n");
    rg_free(test_rg);

    fprintf(output, "\n");

    fprintf(output, "Identifiers:\n");
    FiniteAutomata *fa_id = create_identifier_fa();
    RegularGrammar *rg_id = create_identifier_rg();

    fprintf(output, "\nFA Definition:\n");
    fa_print_file(fa_id, output);

    fprintf(output, "\nRG Definition (manually created):\n");
    rg_print_file(rg_id, output);

    fprintf(output, "\nRG Definition (converted from FA using fa_to_rg):\n");
    RegularGrammar *rg_id_converted = fa_to_rg(fa_id);
    rg_print_file(rg_id_converted, output);

    fprintf(output, "\nVerifying identifiers:\n");
    const char *test_ids[] = {"!a", "!x1", "!test_var", "!A", "!MyVar123", NULL};
    for (int i = 0; test_ids[i] != NULL; i++)
    {
        bool fa_result = fa_verify(fa_id, test_ids[i]);
        fprintf(output, "  '%s': %s\n", test_ids[i], fa_result ? "ACCEPT" : "REJECT");
    }

    fprintf(output, "\nInteger Constants\n");
    FiniteAutomata *fa_int = create_int_constant_fa();
    RegularGrammar *rg_int = create_int_constant_rg();

    fprintf(output, "\nFA Definition:\n");
    fa_print_file(fa_int, output);

    fprintf(output, "\nRG Definition (manually created):\n");
    rg_print_file(rg_int, output);

    fprintf(output, "\nRG Definition (converted from FA using fa_to_rg):\n");
    RegularGrammar *rg_int_converted = fa_to_rg(fa_int);
    rg_print_file(rg_int_converted, output);

    fprintf(output, "\nVerifying integer constants:\n");
    const char *test_ints[] = {"0", "123", "-456", "+789", "1", "42", "01", NULL};
    for (int i = 0; test_ints[i] != NULL; i++)
    {
        bool fa_result = fa_verify(fa_int, test_ints[i]);
        fprintf(output, "  '%s': %s\n", test_ints[i], fa_result ? "ACCEPT" : "REJECT");
    }

    fprintf(output, "\nFloat Constants\n");
    FiniteAutomata *fa_float = create_float_constant_fa();
    RegularGrammar *rg_float = create_float_constant_rg();

    fprintf(output, "\nFA Definition:\n");
    fa_print_file(fa_float, output);

    fprintf(output, "\nRG Definition (manually created):\n");
    rg_print_file(rg_float, output);

    fprintf(output, "\nRG Definition (converted from FA using fa_to_rg):\n");
    RegularGrammar *rg_float_converted = fa_to_rg(fa_float);
    rg_print_file(rg_float_converted, output);

    fprintf(output, "\nVerifying float constants:\n");
    const char *test_floats[] = {"3.14", "-2.5", "+0.5", "123.456", "0.0", NULL};
    for (int i = 0; test_floats[i] != NULL; i++)
    {
        bool fa_result = fa_verify(fa_float, test_floats[i]);
        fprintf(output, "  '%s': %s\n", test_floats[i], fa_result ? "ACCEPT" : "REJECT");
    }

    fprintf(output, "\nCharacter Constants\n");
    FiniteAutomata *fa_char = create_char_constant_fa();
    RegularGrammar *rg_char = create_char_constant_rg();

    fprintf(output, "\nFA Definition:\n");
    fa_print_file(fa_char, output);

    fprintf(output, "\nRG Definition (manually created):\n");
    rg_print_file(rg_char, output);

    fprintf(output, "\nRG Definition (converted from FA using fa_to_rg):\n");
    RegularGrammar *rg_char_converted = fa_to_rg(fa_char);
    rg_print_file(rg_char_converted, output);

    fprintf(output, "\nVerifying character constants:\n");
    const char *test_chars[] = {"'a'", "'7'", "'_'", "'A'", "'0'", NULL};
    for (int i = 0; test_chars[i] != NULL; i++)
    {
        bool fa_result = fa_verify(fa_char, test_chars[i]);
        fprintf(output, "  '%s': %s\n", test_chars[i], fa_result ? "ACCEPT" : "REJECT");
    }

    fprintf(output, "\nString Constants\n");
    FiniteAutomata *fa_string = create_string_constant_fa();
    RegularGrammar *rg_string = create_string_constant_rg();

    fprintf(output, "\nFA Definition:\n");
    fa_print_file(fa_string, output);

    fprintf(output, "\nRG Definition (manually created):\n");
    rg_print_file(rg_string, output);

    fprintf(output, "\nRG Definition (converted from FA using fa_to_rg):\n");
    RegularGrammar *rg_string_converted = fa_to_rg(fa_string);
    rg_print_file(rg_string_converted, output);

    fprintf(output, "\nVerifying string constants:\n");
    const char *test_strings[] = {"\"hello\"", "\"test 123\"", "\"a_b\"", "\"\"", "\"x.y;!\"", NULL};
    for (int i = 0; test_strings[i] != NULL; i++)
    {
        bool fa_result = fa_verify(fa_string, test_strings[i]);
        fprintf(output, "  '%s': %s\n", test_strings[i], fa_result ? "ACCEPT" : "REJECT");
    }

    // Cleanup
    fa_free(fa_id);
    rg_free(rg_id);
    rg_free(rg_id_converted);
    fa_free(fa_int);
    rg_free(rg_int);
    rg_free(rg_int_converted);
    fa_free(fa_float);
    rg_free(rg_float);
    rg_free(rg_float_converted);
    fa_free(fa_char);
    rg_free(rg_char);
    rg_free(rg_char_converted);
    fa_free(fa_string);
    rg_free(rg_string);
    rg_free(rg_string_converted);

    fprintf(output, "\nVerification completed.\n");

    fclose(output);
    printf("Output written to output.txt\n");
    return 0;
}