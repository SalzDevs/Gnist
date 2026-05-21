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

void moveParticle(Vector2 moveVec,Particle *part){
  part->center.x += moveVec.x;
  part->center.y += moveVec.y;
}

void addParticle(Particles *particles, Particle part){
  if (particles->count+1 > particles->size){
    particles->size+=20;
    particles->arr = realloc(particles->arr, particles->size * sizeof(Particles));
  }
  particles->arr[particles->count] = part;
  particles->count++;
}

int main(void) {
  Particles particles;
  initParticles(50,&particles);

  /*spanw the first particle*/ 
  Particle part;
  initParticle(&part, 200, 100);
  addParticle(&particles, part);

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Gnist");

  SetTargetFPS(60);              
  float spawnTimer = 0.0f;
  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    BeginDrawing();
    ClearBackground(RAYWHITE);

    spawnTimer+=dt;
    if (spawnTimer > 5.0f){
      Particle part;
      initParticle(&part, 200, 100);
      addParticle(&particles, part);
      spawnTimer -= 5.0f;
    }

    for (size_t i = 0; i<particles.count; i++) {
      DrawCircle(particles.arr[i].center.x, particles.arr[i].center.y, particles.arr[i].radius, MAROON); 
    }

    EndDrawing();
  }

  CloseWindow();
  free(particles.arr);
  return 0;
}
