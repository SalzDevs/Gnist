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
    Particle particles[1];
    particles[0] = part;
    
    InitWindow(WINDOW_WIDTH,WINDOW_HEIGHT, "Gnist");

    SetTargetFPS(60);              

    size_t length_particles = sizeof(particles)/sizeof(particles[0]);
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        
        for (int i = 0; i<length_particles; i++) {
          DrawCircle(particles[i].particleCircle.center.x, particles[i].particleCircle.center.y, particles[i].particleCircle.radius, MAROON); 
        }

        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
