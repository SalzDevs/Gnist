#include "spawner.h"
#include <math.h>
#include "raylib.h"

static void init_particle(const Spawner *s, Particle *part,
                          float x, float y) {
    part->mass = (float)GetRandomValue((int)s->mass_min,
                                       (int)s->mass_max);
    part->radius       = 3.0f + part->mass * 1.2f;
    part->center       = (float2){x, y};
    part->velocity     = (float2){0, 0};
    part->acceleration = (float2){0, 0};
    part->ttl = (float)GetRandomValue((int)s->lifetime_min,
                                      (int)s->lifetime_max);
    part->max_ttl      = part->ttl;
    float speed = (float)GetRandomValue((int)s->speed_min,
                                        (int)s->speed_max);
    float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
    part->velocity = (float2){cosf(angle) * speed, sinf(angle) * speed};
}

void spawner_init(Spawner *s, float interval) {
    s->interval     = interval;
    s->accumulator  = interval;
    s->lifetime_min = 5.0f;   s->lifetime_max = 15.0f;
    s->speed_min    = 20.0f;  s->speed_max    = 120.0f;
    s->mass_min     = 1.0f;   s->mass_max     = 10.0f;
}

void spawner_set_interval(Spawner *s, float interval) {
    s->interval = interval;
}

float spawner_get_interval(const Spawner *s) {
    return s->interval;
}

void spawner_update(Spawner *s, Pool *pool, float dt,
                    float world_w, float world_h) {
    s->accumulator += dt;
    while (s->accumulator >= s->interval) {
        s->accumulator -= s->interval;
        float x = (float)GetRandomValue(0, (int)world_w);
        float y = (float)GetRandomValue(0, (int)world_h);
        Particle part;
        init_particle(s, &part, x, y);
        pool_push(pool, part);
    }
}
