#include "kv_store.h"

/* ---------- Hash Function ----------*/
unsigned int hash(const char *key) {
    unsigned int h = 0;
    for (int i = 0; key[i]; i++)
        h = (h * 31 + key[i]) % TABLE_SIZE;
    return h;
}