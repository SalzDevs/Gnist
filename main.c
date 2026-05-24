#include <math.h>
#include <stdio.h>
#include "raylib.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-result"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#pragma GCC diagnostic pop
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

    float gravity = 2000.0f;
    bool paused   = false;
    bool settings_open = false;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Gnist");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float dt      = GetFrameTime();
        float world_w = (float)GetScreenWidth();
        float world_h = (float)GetScreenHeight();

        /* ---- controls ---- */
        if (IsKeyPressed(KEY_TAB)) settings_open = !settings_open;
        if (IsKeyPressed(KEY_SPACE) && !settings_open) paused = !paused;
        if (IsKeyPressed(KEY_R) && !settings_open) {
            pool.len = 0;
            spawner.accumulator = spawner.interval;
        }

        /* ---- simulation ---- */
        if (!paused && !settings_open) {
            spawner_update(&spawner, &pool, dt, world_w, world_h);
            physics_update(pool.data, pool.len, dt,
                           world_w, world_h, gravity);
            pool_reap(&pool);
        }

        /* ---- draw ---- */
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (size_t i = 0; i < pool.len; i++) {
            Particle *p = &pool.data[i];
            float frac  = p->ttl / p->max_ttl;
            float r     = p->radius;
            if (frac < 0.3f) r *= frac / 0.3f;
            Color c = ColorFromHSV(p->mass * 24.0f, 0.8f, 0.9f);
            c.a = (unsigned char)(255.0f * frac);
            DrawCircle((int)p->center.x, (int)p->center.y, r, c);
        }

        /* ---- HUD ---- */
        char hud[128];
        snprintf(hud, sizeof(hud),
                 "FPS: %d | particles: %zu | gravity: %.0f | interval: %.1fs | %s",
                 GetFPS(), pool.len, gravity,
                 spawner_get_interval(&spawner),
                 paused ? "PAUSED" : "running");
        DrawText(hud, 10, 10, 18, DARKGRAY);
        DrawText("[TAB:settings] [SPACE:pause] [R:clear]", 10, 32, 14, LIGHTGRAY);

        /* ---- settings overlay ---- */
        if (settings_open) {
            DrawRectangle(0, 0, (int)world_w, (int)world_h,
                          (Color){0, 0, 0, 160});

            float pw = 420.0f, ph = 300.0f;
            float px = (world_w - pw) * 0.5f;
            float py = (world_h - ph) * 0.5f;
            Rectangle panel = {px, py, pw, ph};

            GuiPanel(panel, "Settings");

            float pad  = 24.0f;
            float gap  = 36.0f;
            float y    = py + 48.0f;
            float sw   = pw - pad * 2.0f;

            float interval = spawner_get_interval(&spawner);
            GuiSlider((Rectangle){px + pad, y, sw, 24},
                      "spawn interval", TextFormat("%.1fs", interval),
                      &interval, 0.5f, 10.0f);
            spawner_set_interval(&spawner, interval);

            y += gap;
            GuiSlider((Rectangle){px + pad, y, sw, 24},
                      "gravity", TextFormat("%.0f", gravity),
                      &gravity, 0.0f, 5000.0f);

            y += gap + 8.0f;
            float btn_w = (sw - 12.0f) * 0.5f;
            if (GuiButton((Rectangle){px + pad, y, btn_w, 30}, "reset defaults")) {
                gravity = 2000.0f;
                spawner_set_interval(&spawner, 5.0f);
            }
            if (GuiButton((Rectangle){px + pad + btn_w + 12.0f, y, btn_w, 30}, "clear all")) {
                pool.len = 0;
                spawner.accumulator = spawner.interval;
            }

            y += 46.0f;
            if (GuiButton((Rectangle){px + (pw - 100.0f) * 0.5f, y, 100, 30}, "close"))
                settings_open = false;
        }

        EndDrawing();
    }

    CloseWindow();
    pool_destroy(&pool);
    return 0;
}
