#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define WINDOW_WIDTH 800 
#define WINDOW_HEIGHT 450 

typedef struct {
  bool alive;
  int health;
  Vector2 center;
  int radius;
} Particle;

typedef struct{
  size_t size;
  size_t count;
  Particle* arr;
} Particles;

void initParticle(Particle* part, float x, float y){
  part->alive = true;
  part->health = 100;
  part->radius = 10;
  part->center = (Vector2) {x,y}; 
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

void moveParticle(Particle *part){
    Vector2 move = {GetRandomValue(-10,10),GetRandomValue(-10,10)};
    part->center.x += move.x;
    part->center.y += move.y;
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

    for (size_t i = 0; i<particles.count; i++) {
      DrawCircle(particles.arr[i].center.x, particles.arr[i].center.y, particles.arr[i].radius, MAROON);
      moveParticle(&particles.arr[i]);
    }

    EndDrawing();
  }

  CloseWindow();
  free(particles.arr);
  return 0;
}
