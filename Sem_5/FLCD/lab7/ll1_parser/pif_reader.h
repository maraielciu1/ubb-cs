#ifndef PIF_READER_H
#define PIF_READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int* tokens;
    int count;
    int capacity;
} TokenStream;

// Read PIF file and convert to token stream
TokenStream* read_pif_file(const char* filename);
void free_token_stream(TokenStream* ts);
int token_name_to_code(const char* token_name);

#endif

