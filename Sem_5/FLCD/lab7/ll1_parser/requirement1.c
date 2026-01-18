#include "ll1_parser.h"
#include "arithmetic_grammar.h"
#include <stdio.h>
#include <stdlib.h>

// Requirement 1: Test with arithmetic grammar and sequence
int main(int argc, char **argv)
{
    printf("Requirement 1: LL(1) Parser Test\n");
    printf("================================\n\n");

    // Grammar: E → E + T | T, T → T * F | F, F → (E) | a
    // Converted to LL(1): E → T E', E' → + T E' | ε, T → F T', T' → * F T' | ε, F → (E) | a

    // Test sequence: w = a * (a + a)
    const char *sequence = "a - ( a + a )";

    printf("Grammar: E → T E'\n");
    printf("        E' → + T E' | ε\n");
    printf("        T → F T'\n");
    printf("        T' → * F T' | ε\n");
    printf("        F → (E) | a\n\n");

    printf("Input sequence: %s\n\n", sequence);

    // Create arithmetic grammar
    Grammar *grammar = create_arithmetic_grammar();
    printf("Grammar created with %d productions\n\n", grammar->num_productions);

    // Convert sequence to tokens
    int token_count;
    int *tokens = sequence_to_tokens(sequence, &token_count);

    printf("Token sequence: ");
    for (int i = 0; i < token_count - 1; i++)
    {
        printf("%s ", arithmetic_token_name(tokens[i]));
    }
    printf("$\n\n");

    // Create parser
    LL1Parser *parser = create_ll1_parser(grammar);

    // Parse
    printf("Starting LL(1) parsing...\n\n");
    bool success = parse_arithmetic(parser, tokens, token_count);

    if (success)
    {
        printf("\n✓ Parsing successful!\n\n");

        // Print productions used
        printf("Productions applied (in order):\n");
        // This would be tracked during parsing

        // Print parse tree table
        print_parse_tree_table(parser, "out/requirement1_parse_tree.txt");
        printf("Parse tree table saved to: out/requirement1_parse_tree.txt\n");
    }
    else
    {
        printf("\n✗ Parsing failed!\n");
    }

    // Cleanup
    free(tokens);
    free_parser(parser);
    free_grammar(grammar);

    return success ? 0 : 1;
}
