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

        if (IsKeyPressed(KEY_TAB)) settings_open = !settings_open;
        if (IsKeyPressed(KEY_SPACE) && !settings_open) paused = !paused;
        if (IsKeyPressed(KEY_R) && !settings_open) {
            pool.len = 0;
            spawner.accumulator = spawner.interval;
        }

        if (!paused && !settings_open) {
            spawner_update(&spawner, &pool, dt, world_w, world_h);
            physics_update(pool.data, pool.len, dt,
                           world_w, world_h, gravity);
            pool_reap(&pool);
        }

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

        char hud[128];
        snprintf(hud, sizeof(hud),
                 "FPS: %d | particles: %zu | gravity: %.0f | interval: %.1fs | %s",
                 GetFPS(), pool.len, gravity,
                 spawner_get_interval(&spawner),
                 paused ? "PAUSED" : "running");
        DrawText(hud, 10, 10, 18, DARKGRAY);
        DrawText("[TAB:settings] [SPACE:pause] [R:clear]", 10, 32, 14, LIGHTGRAY);

        if (settings_open) {
            DrawRectangle(0, 0, (int)world_w, (int)world_h,
                          (Color){0, 0, 0, 160});

            float pw = 520.0f, ph = 460.0f;
            float px = (world_w - pw) * 0.5f;
            float py = (world_h - ph) * 0.5f;
            Rectangle panel = {px, py, pw, ph};

            GuiPanel(panel, "Settings");

            float pad   = 24.0f;
            float gap   = 34.0f;
            float y     = py + 50.0f;
            float lbl_w = 80.0f;
            float val_w = 50.0f;
            float bar_w = pw - pad * 2.0f - lbl_w - val_w - 16.0f;

            float interval = spawner_get_interval(&spawner);
            GuiLabel((Rectangle){px + pad, y, lbl_w, 24}, "interval");
            GuiSliderBar((Rectangle){px + pad + lbl_w + 8, y, bar_w, 24},
                         NULL, NULL, &interval, 0.5f, 10.0f);
            GuiLabel((Rectangle){px + pad + lbl_w + bar_w + 16, y, val_w, 24},
                     TextFormat("%.1fs", interval));
            spawner_set_interval(&spawner, interval);

            y += gap;
            GuiLabel((Rectangle){px + pad, y, lbl_w, 24}, "gravity");
            GuiSliderBar((Rectangle){px + pad + lbl_w + 8, y, bar_w, 24},
                         NULL, NULL, &gravity, 0.0f, 5000.0f);
            GuiLabel((Rectangle){px + pad + lbl_w + bar_w + 16, y, val_w, 24},
                     TextFormat("%.0f", gravity));

            y += gap + 4.0f;
            float c_lbl  = 60.0f;
            float c_tag  = 26.0f;
            float c_bar  = 105.0f;
            float c_val  = 36.0f;
            float c_gap  = 6.0f;

            GuiLabel((Rectangle){px + pad, y + 2, c_lbl, 24}, "lifetime");
            GuiLabel((Rectangle){px + pad + c_lbl + c_gap, y + 2, c_tag, 24}, "min");
            GuiSliderBar((Rectangle){px + pad + c_lbl + c_tag + c_gap*2, y, c_bar, 24},
                         NULL, NULL, &spawner.lifetime_min, 1.0f, 30.0f);
            GuiLabel((Rectangle){px + pad + c_lbl + c_tag + c_bar + c_gap*3, y + 2, c_val, 24},
                     TextFormat("%.0f", spawner.lifetime_min));
            GuiLabel((Rectangle){px + pad + c_lbl + c_tag + c_bar + c_val + c_gap*4, y + 2, c_tag, 24}, "max");
            GuiSliderBar((Rectangle){px + pad + c_lbl + c_tag*2 + c_bar + c_val + c_gap*5, y, c_bar, 24},
                         NULL, NULL, &spawner.lifetime_max, 1.0f, 30.0f);
            GuiLabel((Rectangle){px + pad + c_lbl + c_tag*2 + c_bar*2 + c_val + c_gap*6, y + 2, c_val, 24},
                     TextFormat("%.0f", spawner.lifetime_max));

            y += gap;
            GuiLabel((Rectangle){px + pad, y + 2, c_lbl, 24}, "speed");
            GuiLabel((Rectangle){px + pad + c_lbl + c_gap, y + 2, c_tag, 24}, "min");
            GuiSliderBar((Rectangle){px + pad + c_lbl + c_tag + c_gap*2, y, c_bar, 24},
                         NULL, NULL, &spawner.speed_min, 5.0f, 300.0f);
            GuiLabel((Rectangle){px + pad + c_lbl + c_tag + c_bar + c_gap*3, y + 2, c_val, 24},
                     TextFormat("%.0f", spawner.speed_min));
            GuiLabel((Rectangle){px + pad + c_lbl + c_tag + c_bar + c_val + c_gap*4, y + 2, c_tag, 24}, "max");
            GuiSliderBar((Rectangle){px + pad + c_lbl + c_tag*2 + c_bar + c_val + c_gap*5, y, c_bar, 24},
                         NULL, NULL, &spawner.speed_max, 5.0f, 300.0f);
            GuiLabel((Rectangle){px + pad + c_lbl + c_tag*2 + c_bar*2 + c_val + c_gap*6, y + 2, c_val, 24},
                     TextFormat("%.0f", spawner.speed_max));

            y += gap;
            GuiLabel((Rectangle){px + pad, y + 2, c_lbl, 24}, "mass");
            GuiLabel((Rectangle){px + pad + c_lbl + c_gap, y + 2, c_tag, 24}, "min");
            GuiSliderBar((Rectangle){px + pad + c_lbl + c_tag + c_gap*2, y, c_bar, 24},
                         NULL, NULL, &spawner.mass_min, 1.0f, 20.0f);
            GuiLabel((Rectangle){px + pad + c_lbl + c_tag + c_bar + c_gap*3, y + 2, c_val, 24},
                     TextFormat("%.0f", spawner.mass_min));
            GuiLabel((Rectangle){px + pad + c_lbl + c_tag + c_bar + c_val + c_gap*4, y + 2, c_tag, 24}, "max");
            GuiSliderBar((Rectangle){px + pad + c_lbl + c_tag*2 + c_bar + c_val + c_gap*5, y, c_bar, 24},
                         NULL, NULL, &spawner.mass_max, 1.0f, 20.0f);
            GuiLabel((Rectangle){px + pad + c_lbl + c_tag*2 + c_bar*2 + c_val + c_gap*6, y + 2, c_val, 24},
                     TextFormat("%.0f", spawner.mass_max));

            y += gap + 8.0f;
            float btn_w = (pw - pad * 2.0f - 12.0f) * 0.5f;
            if (GuiButton((Rectangle){px + pad, y, btn_w, 30}, "reset defaults")) {
                gravity = 2000.0f;
                spawner_init(&spawner, 5.0f);
            }
            if (GuiButton((Rectangle){px + pad + btn_w + 12, y, btn_w, 30}, "clear all")) {
                pool.len = 0;
                spawner.accumulator = spawner.interval;
            }

            y += 44.0f;
            if (GuiButton((Rectangle){px + (pw - 100.0f) * 0.5f, y, 100, 30}, "close"))
                settings_open = false;
        }

        EndDrawing();
    }

    CloseWindow();
    pool_destroy(&pool);
    return 0;
}
