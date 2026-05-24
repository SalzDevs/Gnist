#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 450
#define GRAVITY       2000.0f
#define SOFTENING     10.0f       /* prevents infinite force when particles overlap */

typedef struct {
  bool    alive;
  int     health;
  Vector2 center;
  float   radius;
  Vector2 velocity;
  Vector2 acceleration;
  float   mass;
} Particle;

typedef struct{
  size_t size;
  size_t count;
  Particle* arr;
} Particles;


void initParticle(Particle* part, float x, float y){
  part->alive        = true;
  part->health       = 100;
  part->mass         = (float)GetRandomValue(1, 10);
  part->radius       = 3.0f + part->mass * 1.2f;   /* bigger mass = bigger circle */
  part->center       = (Vector2){x, y};
  part->velocity     = (Vector2){0};
  part->acceleration = (Vector2){0};
  float speed = (float)GetRandomValue(20, 120);
  float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
  part->velocity = (Vector2){cosf(angle) * speed, sinf(angle) * speed};
}  

void initParticles(size_t size, Particles *particles){
  particles->size = size;
  particles->count = 0;
  particles->arr = malloc(size*sizeof(Particle));
}

void addParticle(Particles *particles, Particle part){
  if (particles->count+1 > particles->size){
    particles->size+=20;
    particles->arr = realloc(particles->arr, particles->size * sizeof(Particle));
  }
  particles->arr[particles->count] = part;
  particles->count++;
}

/* ---- N-body gravity: every particle pulls every other ---- */
void updateParticles(Particle *arr, size_t count, float dt) {
    /* pass 1 — reset accelerations */
    for (size_t i = 0; i < count; i++) {
        arr[i].acceleration = (Vector2){0};
    }

    /* pass 2 — accumulate gravitational pull for every unique pair */
    for (size_t i = 0; i < count; i++) {
        for (size_t j = i + 1; j < count; j++) {
            float dx = arr[j].center.x - arr[i].center.x;
            float dy = arr[j].center.y - arr[i].center.y;
            float dist_sq = dx*dx + dy*dy + SOFTENING;
            float dist    = sqrtf(dist_sq);
            float force   = GRAVITY / dist_sq;
            float fx      = force * dx / dist;
            float fy      = force * dy / dist;

            /* A pulls on B, B pulls on A (equal & opposite) */
            arr[i].acceleration.x += fx * arr[j].mass;
            arr[i].acceleration.y += fy * arr[j].mass;
            arr[j].acceleration.x -= fx * arr[i].mass;
            arr[j].acceleration.y -= fy * arr[i].mass;
        }
    }

    /* pass 3 — integrate acceleration → velocity → position */
    for (size_t i = 0; i < count; i++) {
        arr[i].velocity.x += arr[i].acceleration.x * dt;
        arr[i].velocity.y += arr[i].acceleration.y * dt;
        arr[i].center.x   += arr[i].velocity.x * dt;
        arr[i].center.y   += arr[i].velocity.y * dt;
    }
}

int main(void) {
  Particles particles;
  initParticles(50,&particles);
  float rand_x = (float)GetRandomValue(0, WINDOW_WIDTH);
  int rand_y = (float)GetRandomValue(0, WINDOW_HEIGHT);
  /* spanw the first particle */ 
  Particle part;
  initParticle(&part, rand_x, rand_y);
  addParticle(&particles, part);

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Gnist");
  
  float spawnTimer = 0.0f;
  SetTargetFPS(60);      

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    BeginDrawing();
    ClearBackground(RAYWHITE);

    spawnTimer+=dt;
    if (spawnTimer > 5.0f){
      rand_x = (float)GetRandomValue(0, WINDOW_WIDTH);
      rand_y = (float)GetRandomValue(0, WINDOW_HEIGHT);

      Particle part;
      initParticle(&part, rand_x, rand_y);
      addParticle(&particles, part);
      spawnTimer -= 5.0f;
    }

    updateParticles(particles.arr, particles.count, dt);

    for (size_t i = 0; i < particles.count; i++) {
      float r = particles.arr[i].radius;
      DrawCircle((int)particles.arr[i].center.x, (int)particles.arr[i].center.y, r, MAROON);
    }

    EndDrawing();
  }

  CloseWindow();
  free(particles.arr);
  return 0;
}
