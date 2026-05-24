#include "spawner.h"
#include <math.h>
#include "raylib.h"

static void init_particle(const Spawner *s, Particle *part,
                          float x, float y, float z) {
    part->mass = (float)GetRandomValue((int)s->mass_min,
                                       (int)s->mass_max);
    part->radius       = 3.0f + part->mass * 1.2f;
    part->center       = (float3){x, y, z};
    part->velocity     = (float3){0, 0, 0};
    part->acceleration = (float3){0, 0, 0};
    part->ttl = (float)GetRandomValue((int)s->lifetime_min,
                                      (int)s->lifetime_max);
    part->max_ttl = part->ttl;
    float speed = (float)GetRandomValue((int)s->speed_min,
                                        (int)s->speed_max);
    float angle_y = (float)GetRandomValue(0, 360) * DEG2RAD;
    float angle_z = (float)GetRandomValue(0, 360) * DEG2RAD;
    part->velocity = (float3){
        cosf(angle_y) * cosf(angle_z) * speed,
        sinf(angle_y) * cosf(angle_z) * speed,
        sinf(angle_z) * speed,
    };
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
                    float world_w, float world_h, float world_d) {
    s->accumulator += dt;
    while (s->accumulator >= s->interval) {
        s->accumulator -= s->interval;
        float x = (float)GetRandomValue(0, (int)world_w);
        float y = (float)GetRandomValue(0, (int)world_h);
        float z = (float)GetRandomValue(0, (int)world_d);
        Particle part;
        init_particle(s, &part, x, y, z);
        pool_push(pool, part);
    }
}
