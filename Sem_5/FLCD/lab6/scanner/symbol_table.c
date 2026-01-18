#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define INITIAL_CAPACITY 211
#define LOAD_FACTOR 0.7

typedef struct
{
    char *key;
    int used; // 0 = empty, 1 = occupied
    int index;
} Slot;

static Slot *table = NULL;
static int capacity = 0;
static int size = 0;
static int next_index = 0;

static unsigned long hash1(const char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

static unsigned long hash2(const char *str)
{
    unsigned long hash = 0;
    int c;
    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;
    return (hash % (capacity - 1)) + 1;
}

static int is_prime(int n)
{
    if (n < 2)
        return 0;
    if (n % 2 == 0 && n != 2)
        return 0;
    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0)
            return 0;
    return 1;
}

static int next_prime(int n)
{
    if (n % 2 == 0)
        n++;
    while (!is_prime(n))
        n += 2;
    return n;
}

static void resize()
{
    int old_capacity = capacity;
    Slot *old_table = table;

    capacity = next_prime(capacity * 2);
    table = (Slot *)calloc(capacity, sizeof(Slot));
    size = 0;
    next_index = 0;

    for (int i = 0; i < old_capacity; i++)
    {
        if (old_table[i].used)
        {
            st_insert(old_table[i].key);
            free(old_table[i].key);
        }
    }

    free(old_table);
}

void st_init()
{
    capacity = INITIAL_CAPACITY;
    table = (Slot *)calloc(capacity, sizeof(Slot));
    size = 0;
    next_index = 0;
}

int st_search(const char *value)
{
    unsigned long h1 = hash1(value) % capacity;
    unsigned long h2 = hash2(value);
    int i = 0;
    while (table[(h1 + i * h2) % capacity].used)
    {
        if (strcmp(table[(h1 + i * h2) % capacity].key, value) == 0)
            return 1;
        i++;
        if (i >= capacity)
            return 0;
    }
    return 0;
}

void st_insert(const char *value)
{
    if (!value)
        return;

    if ((double)size / capacity > LOAD_FACTOR)
    {
        resize();
    }

    unsigned long h1 = hash1(value) % capacity;
    unsigned long h2 = hash2(value);
    int i = 0;
    unsigned long pos;

    while (1)
    {
        pos = (h1 + i * h2) % capacity;
        if (!table[pos].used)
        {
            table[pos].key = strdup(value);
            table[pos].used = 1;
            table[pos].index = next_index++;
            size++;
            return;
        }
        else if (strcmp(table[pos].key, value) == 0)
        {
            return;
        }
        i++;
        if (i >= capacity)
            return;
    }
}

int st_get_index(const char *value)
{
    unsigned long h1 = hash1(value) % capacity;
    unsigned long h2 = hash2(value);
    int i = 0;
    while (table[(h1 + i * h2) % capacity].used)
    {
        if (strcmp(table[(h1 + i * h2) % capacity].key, value) == 0)
            return table[(h1 + i * h2) % capacity].index;
        i++;
        if (i >= capacity)
            break;
    }
    return -1;
}

void st_print(const char *filename)
{
    FILE *f = fopen(filename, "w");
    if (!f)
        return;

    for (int i = 0; i < capacity; i++)
    {
        if (table[i].used)
            fprintf(f, "%d: %s\n", table[i].index, table[i].key);
    }

    fclose(f);
}
