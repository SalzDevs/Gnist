#ifndef SPAWNER_H
#define SPAWNER_H

#include "pool.h"

typedef struct {
    float interval;
    float accumulator;
    float world_w;
    float world_h;
} Spawner;

void spawner_init(Spawner *s, float interval, float world_w, float world_h);
void spawner_update(Spawner *s, Pool *pool, float dt);

#endif
