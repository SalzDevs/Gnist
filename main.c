#include <stdio.h>
#include <stdbool.h>
#include "raylib.h"

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
    part->particleCircle.center.x = 300;
    part->particleCircle.center.y = 200;
}

int main(void) {
    Particle part;
    initParticle(&part);

    const int screenWidth = 800;

    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Gnist");

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
