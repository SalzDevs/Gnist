#ifndef POOL_H
#define POOL_H

#include <stddef.h>
#include "physics.h"

typedef struct {
    Particle *data;
    size_t    len;
    size_t    cap;
} Pool;

void pool_init(Pool *p, size_t capacity);
void pool_push(Pool *p, Particle part);
void pool_reap(Pool *p);
void pool_destroy(Pool *p);

#endif
