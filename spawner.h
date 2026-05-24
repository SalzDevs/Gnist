#ifndef SPAWNER_H
#define SPAWNER_H

#include "pool.h"

typedef struct {
    float interval;
    float accumulator;
} Spawner;

void spawner_init(Spawner *s, float interval);
void spawner_update(Spawner *s, Pool *pool, float dt,
                    float world_w, float world_h);
void spawner_set_interval(Spawner *s, float interval);
float spawner_get_interval(const Spawner *s);

#endif
