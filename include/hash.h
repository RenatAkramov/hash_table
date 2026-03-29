#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
struct hash_item
{
    char* key;
    char* value;
    struct hash_item* next;
};

struct hash_table
{
    struct hash_item** item;
    int size;
    int capacity;
};

struct hash_table* hash_table_create(int size);
int    hash_table_destroy(struct hash_table* table);
unsigned long hash_function(char* str);
int hash_table_set(struct hash_table* table, char* key, char* value);
char* hash_table_get(struct hash_table* table, char* key);
int hash_delete(struct hash_table* table, char* key);
int hash_resize(struct hash_table* table, int new_capacity);
int hash_table_upload_from_file(struct hash_table* table, char* filename);

#ifdef __cplusplus
}
#endif

#endif
