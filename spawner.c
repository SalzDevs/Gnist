#include "spawner.h"
#include <math.h>
#include "raylib.h"

static void init_particle(Particle *part, float x, float y) {
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

void spawner_init(Spawner *s, float interval, float world_w, float world_h) {
    s->interval    = interval;
    s->accumulator = interval;  /* fire on first tick */
    s->world_w     = world_w;
    s->world_h     = world_h;
}

void spawner_update(Spawner *s, Pool *pool, float dt) {
    s->accumulator += dt;
    while (s->accumulator >= s->interval) {
        s->accumulator -= s->interval;
        float x = (float)GetRandomValue(0, (int)s->world_w);
        float y = (float)GetRandomValue(0, (int)s->world_h);
        Particle part;
        init_particle(&part, x, y);
        pool_push(pool, part);
    }
}
