#include "physics.h"
#include <math.h>

void physics_update(Particle *arr, size_t count, float dt) {
    /* pass 1 — reset accelerations */
    for (size_t i = 0; i < count; i++) {
        arr[i].acceleration = (float2){0, 0};
    }

    /* pass 2 — accumulate gravitational pull for every unique pair */
    for (size_t i = 0; i < count; i++) {
        for (size_t j = i + 1; j < count; j++) {
            float dx = arr[j].center.x - arr[i].center.x;
            float dy = arr[j].center.y - arr[i].center.y;
            float dist_sq = dx*dx + dy*dy + SOFTENING;
            float dist    = sqrtf(dist_sq);
            float force   = GRAVITY / dist_sq;
            float fx      = force * dx / dist;
            float fy      = force * dy / dist;

            /* A pulls B, B pulls A (equal & opposite) */
            arr[i].acceleration.x += fx * arr[j].mass;
            arr[i].acceleration.y += fy * arr[j].mass;
            arr[j].acceleration.x -= fx * arr[i].mass;
            arr[j].acceleration.y -= fy * arr[i].mass;
        }
    }

    /* pass 3 — integrate acceleration → velocity → position */
    for (size_t i = 0; i < count; i++) {
        arr[i].velocity.x += arr[i].acceleration.x * dt;
        arr[i].velocity.y += arr[i].acceleration.y * dt;
        arr[i].center.x   += arr[i].velocity.x * dt;
        arr[i].center.y   += arr[i].velocity.y * dt;
    }
}
