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
  Particle** arr;
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

void addParticle(Particles *particles, Particle *part){
  if (particles->count+1 > particles->size) particles->size+=20;
  particles->arr[particles->count] = part;
  particles->count++;
}

int main(void) {
  Particles particles;
  Particle part;
  initParticle(&part,100,200);
  initParticles(3,&particles);
  printf("Particles: (count:%zu size:%zu)\n",particles.count,particles.size);
  addParticle(&particles, &part);
  addParticle(&particles, &part);
  addParticle(&particles, &part);
  printf("Particles: (count:%zu size:%zu)\n",particles.count,particles.size); 
  addParticle(&particles, &part);
  addParticle(&particles, &part);
  printf("Particles: (count:%zu size:%zu)\n",particles.count,particles.size);
  for (int i=0;i<particles.count;i++){
    printf("Particle number {%d} (alive:%d health:%d radius:%d x:%f y:%f)\n",i,particles.arr[i]->alive,particles.arr[i]->health,particles.arr[i]->radius,particles.arr[i]->center.x,particles.arr[i]->center.y);
  }
  //moveParticle((Vector2){100,-50}, &particles[0]);
  /*
  size_t length_particles = sizeof(particles)/sizeof(particles[0]);

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Gnist");

  SetTargetFPS(60);              


  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int i = 0; i<length_particles; i++) {
      DrawCircle(particles[i].center.x, particles[i].center.y, particles[i].radius, MAROON); 
    }

    EndDrawing();
  }

  CloseWindow();
  */
  return 0;
}
