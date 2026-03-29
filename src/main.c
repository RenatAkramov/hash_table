#include "../include/hash.h"

void print_help() {
    printf("\nAvailable commands:\n");
    printf("  upload <filename>  - Add file\n");
    printf("  set <key> <value>  - Add or update an entry\n");
    printf("  get <key>          - Find a value by key\n");
    printf("  del <key>          - Delete a key\n");
    printf("  help               - Show this message\n");
    printf("  exit               - Close the program\n");
}

int main() {
    struct hash_table* table = hash_table_create(16);
    if (!table) return 1;

    char line[512]; 
    print_help();

    while (true) 
    {
        printf("ht_shell> ");
        if (!fgets(line, sizeof(line), stdin)) break;

        line[strcspn(line, "\n")] = 0;

        char* cmd = strtok(line, " \t\r");
        if (!cmd) continue;

        if (strcmp(cmd, "exit") == 0) 
        {
            break;
        } 
        else if (strcmp(cmd, "upload") == 0) 
        {
            char* filename = strtok(NULL, " \t\r");
            if (filename) 
            {
                if (!hash_table_upload_from_file(table, filename)) 
                {
                    printf("Could not process file: %s\n", filename);
                } 
                else 
                {
                    printf("File %s uploaded successfully. Size: %d\n", filename, table->size);
                }
            }
        }
        else if (strcmp(cmd, "set") == 0) 
        {
            char* key = strtok(NULL, " \t\r");
            char* val = strtok(NULL, " \t\r");
            if (key && val) {
                hash_table_set(table, key, val);
                printf("OK. Size: %d\n", table->size);
            } else {
                printf("Usage: set <key> <value>\n");
            }
        } 
        else if (strcmp(cmd, "get") == 0) 
        {
            char* key = strtok(NULL, " \t\r");
            if (key) 
            {
                char* res = hash_table_get(table, key);
                if (res) printf("'%s' => '%s'\n", key, res);
                else printf("Key '%s' not found.\n", key);
            }
        } 
        else if (strcmp(cmd, "del") == 0) 
        {
            char* key = strtok(NULL, " \t\r");
            if (key) 
            {
                if (hash_delete(table, key)) printf("Deleted '%s'.\n", key);
                else printf("Key '%s' not found.\n", key);
            }
        } 
        else if (strcmp(cmd, "help") == 0) 
        {
            print_help();
        }
        else 
        {
            printf("Unknown command: %s. Type 'help'\n", cmd);
        }
    }

    hash_table_destroy(table);
    return 0;
}

