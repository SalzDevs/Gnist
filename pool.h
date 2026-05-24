#ifndef POOL_H
#define POOL_H

#include <stddef.h>
#include "physics.h"   /* Particle */

/* Dynamic array of Particle.  Callers read .len and .data freely;
 * .cap is managed by pool_push — treat it as private. */
typedef struct {
    Particle *data;
    size_t    len;
    size_t    cap;
} Pool;

void pool_init(Pool *p, size_t capacity);
void pool_push(Pool *p, Particle part);
void pool_destroy(Pool *p);

#endif
