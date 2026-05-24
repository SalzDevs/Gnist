#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "physics.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 450

typedef struct {
    size_t    size;
    size_t    count;
    Particle *arr;
} Particles;

void initParticle(Particle *part, float x, float y) {
    part->mass         = (float)GetRandomValue(1, 10);
    part->radius       = 3.0f + part->mass * 1.2f;
    part->center       = (float2){x, y};
    part->velocity     = (float2){0, 0};
    part->acceleration = (float2){0, 0};
    float speed = (float)GetRandomValue(20, 120);
    float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
    part->velocity = (float2){cosf(angle) * speed, sinf(angle) * speed};
}

void initParticles(size_t size, Particles *particles) {
    particles->size  = size;
    particles->count = 0;
    particles->arr   = malloc(size * sizeof(Particle));
}

void addParticle(Particles *particles, Particle part) {
    if (particles->count + 1 > particles->size) {
        particles->size += 20;
        particles->arr = realloc(particles->arr,
                                 particles->size * sizeof(Particle));
    }
    particles->arr[particles->count] = part;
    particles->count++;
}

int main(void) {
    Particles particles;
    initParticles(50, &particles);

    /* spawn the first particle */
    float rand_x = (float)GetRandomValue(0, WINDOW_WIDTH);
    float rand_y = (float)GetRandomValue(0, WINDOW_HEIGHT);
    Particle part;
    initParticle(&part, rand_x, rand_y);
    addParticle(&particles, part);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Gnist");
    SetTargetFPS(60);

    float spawnTimer = 0.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        /* spawn a new particle every 5 seconds */
        spawnTimer += dt;
        if (spawnTimer > 5.0f) {
            rand_x = (float)GetRandomValue(0, WINDOW_WIDTH);
            rand_y = (float)GetRandomValue(0, WINDOW_HEIGHT);
            Particle part;
            initParticle(&part, rand_x, rand_y);
            addParticle(&particles, part);
            spawnTimer -= 5.0f;
        }

        /* physics — pure computation, no raylib */
        physics_update(particles.arr, particles.count, dt);

        /* draw — the render seam: float2 → raylib coords */
        for (size_t i = 0; i < particles.count; i++) {
            Particle *p = &particles.arr[i];
            DrawCircle((int)p->center.x, (int)p->center.y,
                       p->radius, MAROON);
        }

        EndDrawing();
    }

    CloseWindow();
    free(particles.arr);
    return 0;
}
