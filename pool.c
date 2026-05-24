#include "pool.h"
#include <stdlib.h>

void pool_init(Pool *p, size_t capacity) {
    p->data = malloc(capacity * sizeof(Particle));
    p->len  = 0;
    p->cap  = capacity;
}

void pool_push(Pool *p, Particle part) {
    if (p->len >= p->cap) {
        p->cap += 20;
        p->data = realloc(p->data, p->cap * sizeof(Particle));
    }
    p->data[p->len] = part;
    p->len++;
}

void pool_reap(Pool *p) {
    size_t i = 0;
    while (i < p->len) {
        if (p->data[i].ttl <= 0.0f) {
            p->data[i] = p->data[p->len - 1];
            p->len--;
        } else {
            i++;
        }
    }
}

void pool_destroy(Pool *p) {
    free(p->data);
    p->data = NULL;
    p->len  = 0;
    p->cap  = 0;
}
