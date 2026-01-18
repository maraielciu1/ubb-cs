#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

void st_init();

int st_search(const char *value);

void st_insert(const char *value);

int st_get_index(const char *value);

void st_print(const char *filename);

#endif
