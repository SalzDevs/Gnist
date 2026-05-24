#ifndef PHYSICS_H
#define PHYSICS_H

#include <stddef.h>

typedef struct {
    float x, y;
} float2;

typedef struct {
    float2 center;
    float  radius;
    float2 velocity;
    float2 acceleration;
    float  mass;
    float  ttl;
    float  max_ttl;
} Particle;

#define GRAVITY   2000.0f
#define SOFTENING 10.0f

void physics_update(Particle *arr, size_t count, float dt,
                    float world_w, float world_h);

#endif
