#ifndef SPAWNER_H
#define SPAWNER_H

#include "pool.h"

typedef struct {
    float interval;
    float accumulator;
    float lifetime_min, lifetime_max;
    float speed_min,    speed_max;
    float mass_min,     mass_max;
} Spawner;

void  spawner_init(Spawner *s, float interval);
void  spawner_update(Spawner *s, Pool *pool, float dt,
                     float world_w, float world_h, float world_d);
void  spawner_set_interval(Spawner *s, float interval);
float spawner_get_interval(const Spawner *s);

#endif
