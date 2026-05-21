#include <stdbool.h>
#include <stdio.h>
#include "raylib.h"

#define WINDOW_WIDTH 800 
#define WINDOW_HEIGHT 450 

typedef struct {
  bool alive;
  int health;
  Vector2 center;
  int radius;
} Particle;

void initParticle(Particle* part, float x, float y){
  part->alive = true;
  part->health = 100;
  part->radius = 10;
  part->center = (Vector2) {x,y}; 
}

void moveParticle(Vector2 moveVec,Particle *part){
  part->center.x += moveVec.x;
  part->center.y += moveVec.y;
}

int main(void) {
  Particle particles[1];

  initParticle(&particles[0],WINDOW_WIDTH/2,WINDOW_HEIGHT/2);
  moveParticle((Vector2){100,-50}, &particles[0]);
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
  return 0;
}
