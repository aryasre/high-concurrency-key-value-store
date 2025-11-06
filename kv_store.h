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
#define BUF_SIZE   128
#define MAX_WRITER  3
#define MAX_READER  2 
#define WAL_FILE   "wal.log"

typedef struct Node {
    char key[KEY_SIZE];
    char value[VALUE_SIZE];
    struct Node *next;
} Node;

Node *hash_table[TABLE_SIZE];

/* ---------- Initialising Locks ----------*/
pthread_rwlock_t rw_lock = PTHREAD_RWLOCK_INITIALIZER;
pthread_mutex_t wal_lock = PTHREAD_MUTEX_INITIALIZER;

atomic_int write_count = 0;

unsigned int hash(const char *key);
void put(const char *key, const char *value);
char *get(const char *key);
void wal_append(const char *key, const char *value);
void wal_replay();
void *writer_thread(void *arg);
void *reader_thread(void *arg);
void display_table();

#endif 