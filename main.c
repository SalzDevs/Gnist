#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "physics.h"
#include "pool.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 450

void initParticle(Particle *part, float x, float y) {
    part->mass         = (float)GetRandomValue(1, 10);
    part->radius       = 3.0f + part->mass * 1.2f;
    part->center       = (float2){x, y};
    part->velocity     = (float2){0, 0};
    part->acceleration = (float2){0, 0};
    part->ttl          = (float)GetRandomValue(5, 15);
    float speed = (float)GetRandomValue(20, 120);
    float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
    part->velocity = (float2){cosf(angle) * speed, sinf(angle) * speed};
}

int main(void) {
    Pool pool;
    pool_init(&pool, 50);

    float rand_x = (float)GetRandomValue(0, WINDOW_WIDTH);
    float rand_y = (float)GetRandomValue(0, WINDOW_HEIGHT);
    Particle part;
    initParticle(&part, rand_x, rand_y);
    pool_push(&pool, part);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Gnist");
    SetTargetFPS(60);

    float spawnTimer = 0.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        spawnTimer += dt;
        if (spawnTimer > 5.0f) {
            rand_x = (float)GetRandomValue(0, WINDOW_WIDTH);
            rand_y = (float)GetRandomValue(0, WINDOW_HEIGHT);
            Particle part;
            initParticle(&part, rand_x, rand_y);
            pool_push(&pool, part);
            spawnTimer -= 5.0f;
        }

        physics_update(pool.data, pool.len, dt);
        pool_reap(&pool);

        for (size_t i = 0; i < pool.len; i++) {
            Particle *p = &pool.data[i];
            DrawCircle((int)p->center.x, (int)p->center.y,
                       p->radius, MAROON);
        }

        EndDrawing();
    }

    CloseWindow();
    pool_destroy(&pool);
    return 0;
}
