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
    part->particleCircle.center.x = 0;
    part->particleCircle.center.y = 0;
}

int main(void) {
    const int screenWidth = 800;

    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Gnist");

    SetTargetFPS(60);               

    while (!WindowShouldClose()) {
        BeginDrawing();

        

        ClearBackground(RAYWHITE);

        DrawText("Congrats! You just entered Gnist!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
