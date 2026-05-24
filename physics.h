#ifndef PHYSICS_H
#define PHYSICS_H

#include <stddef.h>

/* project-owned 2D vector — no raylib dependency */
typedef struct {
    float x, y;
} float2;

typedef struct {
    float2 center;
    float  radius;
    float2 velocity;
    float2 acceleration;
    float  mass;
} Particle;

#define GRAVITY   2000.0f
#define SOFTENING 10.0f   /* prevents infinite force when particles overlap */

/* N-body gravity: every particle pulls every other.
 * Pure computation — no I/O, no allocation, no raylib. */
void physics_update(Particle *arr, size_t count, float dt);

#endif
