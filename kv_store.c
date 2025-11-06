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

/* ----------- GET Operation ---------- */
char *get(const char *key) {
    unsigned int index = hash(key); 
    pthread_rwlock_rdlock(&rw_lock);
    sleep(2);
    Node *cur = hash_table[index];
    while (cur) {
        if (strcmp(cur->key, key) == 0) {
            pthread_rwlock_unlock(&rw_lock);
            return cur->value;
        }
        cur = cur->next;
    }

    pthread_rwlock_unlock(&rw_lock);
    return NULL;
}

/*---------- Writer Thread function ----------*/
void *writer_thread(void *arg) {
    int id = *(int *)arg;
    char key[KEY_SIZE], value[VALUE_SIZE];

    for (int i = 0; i < 4; i++) {
        snprintf(key, sizeof(key), "key %d_%d", id, i);
        int res_value = (id * 1000) + i;
        snprintf(value, sizeof(value), "%d", res_value);

        wal_append(key, value);
        put(key, value);

        printf("[Writer %d] Wrote [%s : %s]\n", id, key, value);
        sleep(2);
    }

    return NULL;
}

/*---------- Writer Thread function ----------*/
void *writer_thread(void *arg) {
    int id = *(int *)arg;
    char key[KEY_SIZE], value[VALUE_SIZE];

    for (int i = 0; i < 4; i++) {
        //pthread_rwlock_wrlock(&rw_lock);
        snprintf(key, sizeof(key), "key %d_%d", id, i);
        int res_value = (id * 1000) + i;
        snprintf(value, sizeof(value), "%d", res_value);

        wal_append(key, value);
        put(key, value);

        printf("[Writer %d] Wrote [%s : %s]\n", id, key, value);
        //pthread_rwlock_unlock(&rw_lock);
        sleep(2);
    }

    return NULL;
}

/* ---------- Reader Threads ---------- */
void *reader_thread(void *arg) {
    int id = *(int *)arg;
    const char *keys_to_read[] = {"key 1_1","key 1_0", "key 4_2", "key3_0", NULL};

    for (int i = 0; keys_to_read[i]; i++) {
       // pthread_rwlock_rdlock(&rw_lock);
        char *val = get(keys_to_read[i]);
        if (val)
            printf("[Reader %d] %s -> %s\n", id, keys_to_read[i], val);
        else
            printf("[Reader %d] %s -> (key not found)\n", id, keys_to_read[i]);
         //pthread_rwlock_unlock(&rw_lock);
        sleep(1);
    }

    return NULL;
}

/* ---------- Display Hash table---------- */
void display_table() {
    printf("\n--- Hash Table ---\n");
    printf("Index\tKey-Value\t\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *cur = hash_table[i];
        if (cur) {
            printf("[%d]: ", i);
            while (cur) {
                printf("(%s : %s) -> ", cur->key, cur->value);
                cur = cur->next;
            }
            printf("NULL\n");
        }
    }
}

/*-------- Main --------*/
int main() {

    printf("----------Key-Value Store--------------------\n");
   
    display_table(); 

    pthread_t writers[MAX_WRITER], readers[MAX_READER]; 
    int wid[MAX_WRITER] = {1, 2, 3}, rid[MAX_READER] = {1, 2};

    for (int i = 0; i < MAX_WRITER; i++)
        pthread_create(&writers[i], NULL, writer_thread, &wid[i]);
    for (int i = 0; i < MAX_READER; i++)
        pthread_create(&readers[i], NULL, reader_thread, &rid[i]);

    for (int i = 0; i < 3; i++)
        pthread_join(writers[i], NULL);
    for (int i = 0; i < 2; i++)
        pthread_join(readers[i], NULL);

    
    return 0;
}