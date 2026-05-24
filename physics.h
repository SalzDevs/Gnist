#ifndef PHYSICS_H
#define PHYSICS_H

#include <stddef.h>

typedef struct {
    float x, y, z;
} float3;

typedef struct {
    float3 center;
    float  radius;
    float3 velocity;
    float3 acceleration;
    float  mass;
    float  ttl;
    float  max_ttl;
} Particle;

#define SOFTENING   10.0f
#define RESTITUTION 0.9f

void physics_update(Particle *arr, size_t count, float dt,
                    float world_w, float world_h, float world_d,
                    float gravity);

#endif
