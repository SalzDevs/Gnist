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

void pool_destroy(Pool *p) {
    free(p->data);
    p->data = NULL;
    p->len  = 0;
    p->cap  = 0;
}
