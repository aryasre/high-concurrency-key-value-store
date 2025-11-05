#include "kv_store.h"

/* ---------- Hash Function ----------*/
unsigned int hash(const char *key) {
    unsigned int h = 0;
    for (int i = 0; key[i]; i++)
        h = (h * 31 + key[i]) % TABLE_SIZE;
    return h;
}

/* ---------- PUT Operation----------*/
void put(const char *key, const char *value) {
    unsigned int index = hash(key); 
    pthread_rwlock_wrlock(&rw_lock);
    sleep(2);
    Node *cur = hash_table[index];
    while (cur) {
        if (strcmp(cur->key, key) == 0) {  
            strcpy(cur->value, value);
            pthread_rwlock_unlock(&rw_lock);
            return;
        }
        cur = cur->next;
    }

    Node *newNode = malloc(sizeof(Node));
    strcpy(newNode->key, key);
    strcpy(newNode->value, value);
    newNode->next = hash_table[index];
    hash_table[index] = newNode;
     atomic_fetch_add(&write_count, 1);
    pthread_rwlock_unlock(&rw_lock);
}