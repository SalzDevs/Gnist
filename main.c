#include <stdio.h>
#include <stdbool.h>
#include "raylib.h"

#define WINDOW_WIDTH 800 
#define WINDOW_HEIGHT 450 

typedef struct{
  int x;
  int y;
} ParticleCenter;

typedef struct{
  ParticleCenter center; 
  int radius;
} ParticleCircle;

typedef struct {
  bool alive;
  int health;
  ParticleCircle particleCircle;
} Particle;

void initParticle(Particle* part){
    part->alive = true;
    part->health = 100;
    part->particleCircle.radius = 10;
    part->particleCircle.center.x = WINDOW_WIDTH/2;
    part->particleCircle.center.y = WINDOW_HEIGHT/2;
}

int main(void) {
    Particle part;
    initParticle(&part);

    InitWindow(WINDOW_WIDTH,WINDOW_HEIGHT, "Gnist");

    SetTargetFPS(60);               

    while (!WindowShouldClose()) {
        BeginDrawing();

        DrawCircle(part.particleCircle.center.x, part.particleCircle.center.y, part.particleCircle.radius, MAROON); 

        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
