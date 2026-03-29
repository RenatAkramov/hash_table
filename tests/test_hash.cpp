#include <gtest/gtest.h>
#include "../include/hash.h"

TEST(HashTableLogic, GetFunctionality) 
{
    struct hash_table* table = hash_table_create(16);
    
    ASSERT_NE(table, nullptr); 

    hash_table_set(table, (char*)"velocity", (char*)"29979245800");

    char* result = hash_table_get(table, (char*)"velocity");
    ASSERT_NE(result, nullptr);          
    EXPECT_STREQ(result, "29979245800"); 

    EXPECT_EQ(hash_table_get(table, (char*)"unknown"), nullptr);

    hash_table_destroy(table);
}


TEST(HashTableCore, Lifecycle) 
{
    struct hash_table* table = hash_table_create(8);
    
    ASSERT_NE(table, nullptr);      
    ASSERT_NE(table->item, nullptr); 
    EXPECT_EQ(table->size, 0);       
    EXPECT_EQ(table->capacity, 8);   
    
    EXPECT_EQ(hash_table_destroy(table), 1);
}

TEST(HashTableCore, SetAndGet) 
{
    struct hash_table* table = hash_table_create(16);
    
    hash_table_set(table, (char*)"G", (char*)"6.67430e-8");
    
    char* val = hash_table_get(table, (char*)"G");
    ASSERT_NE(val, nullptr);
    EXPECT_STREQ(val, "6.67430e-8");
    
    hash_table_set(table, (char*)"G", (char*)"new_value");
    EXPECT_STREQ(hash_table_get(table, (char*)"G"), "new_value");
    EXPECT_EQ(table->size, 1); 
    
    hash_table_destroy(table);
}

TEST(HashTableCore, Deletion) 
{
    struct hash_table* table = hash_table_create(8);
    
    hash_table_set(table, (char*)"temp", (char*)"300");
    EXPECT_EQ(table->size, 1);
    
    EXPECT_EQ(hash_delete(table, (char*)"temp"), 1);
    EXPECT_EQ(table->size, 0);
    EXPECT_EQ(hash_table_get(table, (char*)"temp"), nullptr);
    
    EXPECT_EQ(hash_delete(table, (char*)"none"), 0);
    
    hash_table_destroy(table);
}

TEST(HashTableLogic, StressResize) 
{
    struct hash_table* table = hash_table_create(2);
    const int num_elements = 100;
    char key[20];
    char val[20];

    for (int i = 0; i < num_elements; i++) 
    {
        sprintf(key, "phys_const_%d", i);
        sprintf(val, "%d", i * 10);
        hash_table_set(table, key, val);
    }

    EXPECT_GT(table->capacity, 100);
    EXPECT_EQ(table->size, num_elements);

    for (int i = 0; i < num_elements; i++) 
    {
        sprintf(key, "phys_const_%d", i);
        sprintf(val, "%d", i * 10);
        char* result = hash_table_get(table, key);
        
        ASSERT_NE(result, nullptr) << "Failed to find key: " << key;
        EXPECT_STREQ(result, val) << "Value mismatch for key: " << key;
    }

    hash_table_destroy(table);
}

TEST(HashTableLogic, CollisionHandling) 
{
    struct hash_table* table = hash_table_create(1); 
    
    hash_table_set(table, (char*)"keyA", (char*)"1");
    hash_table_set(table, (char*)"keyB", (char*)"2");
    hash_table_set(table, (char*)"keyC", (char*)"3");
    
    EXPECT_STREQ(hash_table_get(table, (char*)"keyA"), "1");
    EXPECT_STREQ(hash_table_get(table, (char*)"keyB"), "2");
    EXPECT_STREQ(hash_table_get(table, (char*)"keyC"), "3");
    
    hash_table_destroy(table);
}