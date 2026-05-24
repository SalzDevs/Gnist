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
    spawner_init(&spawner, 5.0f, WINDOW_WIDTH, WINDOW_HEIGHT);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Gnist");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        spawner_update(&spawner, &pool, dt);
        physics_update(pool.data, pool.len, dt,
                       WINDOW_WIDTH, WINDOW_HEIGHT);
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
