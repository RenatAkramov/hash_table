#include "../include/hash.h"

unsigned long hash_function(char* key) 
{
    unsigned long hash = 5381;
    int c;
    while ((c = *key++))
        hash = ((hash << 5) + hash) + c; 
    return hash;
}

unsigned long get_index(char* key, struct hash_table* hash_table)
{
    unsigned long hash  = hash_function(key); 
    unsigned long index = hash % hash_table->capacity;
    return index;
}

struct hash_table* hash_table_create(int start_size)
{
    struct hash_table* table = calloc(1, sizeof(struct hash_table));
    if (!table)
    {
        perror("Failed to allocate memory for hash table");
        return NULL;
    }

    table->capacity = start_size;
    table->size = 0;

    table->item = calloc(table->capacity, sizeof(struct hash_item*));
    
    if (!table->item) 
    {
        free(table);
        return NULL;
    }

    return table;
}

int hash_table_set(struct hash_table* table, char* key, char* value)
{
    unsigned long index = get_index(key, table);   
    struct hash_item* current = table->item[index];
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0) 
        {
            free(current->value);
            current->value = strdup(value);
            return 1; 
        }
        current = current->next;
    }
    struct hash_item* new_item = calloc(1, sizeof(struct hash_item));
    if (!new_item) return 0;
    new_item->key = strdup(key);
    new_item->value = strdup(value);
    new_item->next = table->item[index];
    table->item[index] = new_item;
    table->size++;
    float load_factor = (float)table->size / table->capacity;
    if (load_factor > 0.7)
    {
        if (!hash_resize(table, table->capacity * 2)) return 0;
    }
    return 1;
}
char* hash_table_get(struct hash_table* table, char* key)
{
    unsigned long index = get_index(key, table);   
    struct hash_item* current = table->item[index];
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0) 
        {
            return current->value; 
        }
        current = current->next;
    }
    return NULL;
}

int hash_delete(struct hash_table* table, char* key)
{
    unsigned long index = get_index(key, table);   
    struct hash_item* current = table->item[index];
    struct hash_item* prev = NULL;
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0) 
        {
            if (prev == NULL) 
            {
                table->item[index] = current->next;
            } 
            else 
            {
                prev->next = current->next;
            }
            free(current->key);
            free(current->value);
            free(current);
            table->size--;
            return 1; 
        }
        prev = current;
        current = current->next;
    }
    return 0; 
}

int hash_resize(struct hash_table* table, int new_capacity)
{
    struct hash_item** new_items = calloc(new_capacity, sizeof(struct hash_item*));
    if (!new_items)
    {
        perror("Failed to allocate memory for resizing");
        return 0;
    }

    for (int i = 0; i < table->capacity; i++)
    {
        struct hash_item* current = table->item[i];
        while (current != NULL)
        {
            unsigned long new_index = hash_function(current->key) % new_capacity;
            struct hash_item* next = current->next;
            current->next = new_items[new_index];
            new_items[new_index] = current;
            current = next;
        }
    }
    free(table->item);
    table->item = new_items;
    table->capacity = new_capacity;
    return 1;
}

int hash_table_destroy(struct hash_table* table)
{
    for (int i = 0; i < table->capacity; i++)
    {
        struct hash_item* current = table->item[i];
        while (current != NULL)
        {
            struct hash_item* next = current->next;
            free(current->key);
            free(current->value);
            free(current);
            current = next;
        }
    }
    free(table->item);
    free(table);
    return 1;
}

int hash_table_upload_from_file(struct hash_table* table, char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file) 
    {
        perror("Error opening file");
        return 0;
    }

    char line[256];
    while(fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0;
        char* key = strtok(line, " ");
        char* value = strtok(NULL, " ");

        if (key != NULL && value != NULL)
        {
            hash_table_set(table, key, value);
        }
    }
    fclose(file);
    return 1;
}
