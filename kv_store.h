#ifndef KV_STORE_H
#define KV_STORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdatomic.h>

#define TABLE_SIZE 10
#define KEY_SIZE   32
#define VALUE_SIZE 64

typedef struct Node {
    char key[KEY_SIZE];
    char value[VALUE_SIZE];
    struct Node *next;
} Node;

Node *hash_table[TABLE_SIZE];

unsigned int hash(const char *key);

#endif 