#include "physics.h"
#include <math.h>

void physics_update(Particle *arr, size_t count, float dt,
                    float world_w, float world_h, float world_d,
                    float gravity) {
    for (size_t i = 0; i < count; i++) {
        arr[i].acceleration = (float3){0, 0, 0};
    }

    for (size_t i = 0; i < count; i++) {
        for (size_t j = i + 1; j < count; j++) {
            float dx = arr[j].center.x - arr[i].center.x;
            float dy = arr[j].center.y - arr[i].center.y;
            float dz = arr[j].center.z - arr[i].center.z;
            float dist_sq = dx*dx + dy*dy + dz*dz + SOFTENING;
            float dist    = sqrtf(dist_sq);
            float force   = gravity / dist_sq;
            float fx      = force * dx / dist;
            float fy      = force * dy / dist;
            float fz      = force * dz / dist;

            arr[i].acceleration.x += fx * arr[j].mass;
            arr[i].acceleration.y += fy * arr[j].mass;
            arr[i].acceleration.z += fz * arr[j].mass;
            arr[j].acceleration.x -= fx * arr[i].mass;
            arr[j].acceleration.y -= fy * arr[i].mass;
            arr[j].acceleration.z -= fz * arr[i].mass;
        }
    }

    for (size_t i = 0; i < count; i++) {
        arr[i].velocity.x += arr[i].acceleration.x * dt;
        arr[i].velocity.y += arr[i].acceleration.y * dt;
        arr[i].velocity.z += arr[i].acceleration.z * dt;
        arr[i].center.x   += arr[i].velocity.x * dt;
        arr[i].center.y   += arr[i].velocity.y * dt;
        arr[i].center.z   += arr[i].velocity.z * dt;
    }

    for (size_t i = 0; i < count; i++) {
        for (size_t j = i + 1; j < count; j++) {
            float dx = arr[j].center.x - arr[i].center.x;
            float dy = arr[j].center.y - arr[i].center.y;
            float dz = arr[j].center.z - arr[i].center.z;
            float dist = sqrtf(dx*dx + dy*dy + dz*dz);
            float min_dist = arr[i].radius + arr[j].radius;
            if (dist >= min_dist || dist < 0.0001f) continue;

            float nx = dx / dist;
            float ny = dy / dist;
            float nz = dz / dist;
            float dvx = arr[i].velocity.x - arr[j].velocity.x;
            float dvy = arr[i].velocity.y - arr[j].velocity.y;
            float dvz = arr[i].velocity.z - arr[j].velocity.z;
            float vn = dvx * nx + dvy * ny + dvz * nz;
            if (vn < 0.0f) {
                float inv_mass_sum = 1.0f/arr[i].mass + 1.0f/arr[j].mass;
                float impulse = -(1.0f + RESTITUTION) * vn / inv_mass_sum;
                arr[i].velocity.x += impulse / arr[i].mass * nx;
                arr[i].velocity.y += impulse / arr[i].mass * ny;
                arr[i].velocity.z += impulse / arr[i].mass * nz;
                arr[j].velocity.x -= impulse / arr[j].mass * nx;
                arr[j].velocity.y -= impulse / arr[j].mass * ny;
                arr[j].velocity.z -= impulse / arr[j].mass * nz;
            }

            float overlap = min_dist - dist;
            float total_mass = arr[i].mass + arr[j].mass;
            arr[i].center.x -= nx * overlap * arr[j].mass / total_mass;
            arr[i].center.y -= ny * overlap * arr[j].mass / total_mass;
            arr[i].center.z -= nz * overlap * arr[j].mass / total_mass;
            arr[j].center.x += nx * overlap * arr[i].mass / total_mass;
            arr[j].center.y += ny * overlap * arr[i].mass / total_mass;
            arr[j].center.z += nz * overlap * arr[i].mass / total_mass;
        }
    }

    for (size_t i = 0; i < count; i++) {
        arr[i].ttl -= dt;
        float r = arr[i].radius;
        if (arr[i].center.x < r)             { arr[i].center.x = r;               arr[i].velocity.x *= -1.0f; }
        if (arr[i].center.x > world_w - r)   { arr[i].center.x = world_w - r;     arr[i].velocity.x *= -1.0f; }
        if (arr[i].center.y < r)             { arr[i].center.y = r;               arr[i].velocity.y *= -1.0f; }
        if (arr[i].center.y > world_h - r)   { arr[i].center.y = world_h - r;     arr[i].velocity.y *= -1.0f; }
        if (arr[i].center.z < r)             { arr[i].center.z = r;               arr[i].velocity.z *= -1.0f; }
        if (arr[i].center.z > world_d - r)   { arr[i].center.z = world_d - r;     arr[i].velocity.z *= -1.0f; }
    }
}
