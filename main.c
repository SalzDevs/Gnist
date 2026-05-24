#include <math.h>
#include "raylib.h"
#include "physics.h"
#include "pool.h"
#include "spawner.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 450

int main(void) {
    Pool pool;
    pool_init(&pool, 50);

    Spawner spawner;
    spawner_init(&spawner, 5.0f);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Gnist");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        float world_w = (float)GetScreenWidth();
        float world_h = (float)GetScreenHeight();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        spawner_update(&spawner, &pool, dt, world_w, world_h);
        physics_update(pool.data, pool.len, dt, world_w, world_h);
        pool_reap(&pool);

        for (size_t i = 0; i < pool.len; i++) {
            Particle *p = &pool.data[i];
            float frac  = p->ttl / p->max_ttl;
            float r     = p->radius;
            if (frac < 0.3f) r *= frac / 0.3f;
            Color c = ColorFromHSV(p->mass * 24.0f, 0.8f, 0.9f);
            c.a = (unsigned char)(255.0f * frac);
            DrawCircle((int)p->center.x, (int)p->center.y, r, c);
        }

        EndDrawing();
    }

    CloseWindow();
    pool_destroy(&pool);
    return 0;
}
