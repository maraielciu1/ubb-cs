#ifndef ARITHMETIC_GRAMMAR_H
#define ARITHMETIC_GRAMMAR_H

#include "ll1_parser.h"

Grammar* create_arithmetic_grammar(void);
int* sequence_to_tokens(const char* sequence, int* token_count);
const char* arithmetic_token_name(int code);
bool parse_arithmetic(LL1Parser* parser, int* input_tokens, int input_len);

#endif

