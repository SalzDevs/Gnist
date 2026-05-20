#include <stdio.h>
#include <stdbool.h>

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
    Particle part;
    initParticle(&part);
    printf("particle (alive:%d | health:%d | x:%d | y:%d | radius:%d)\n", part.alive,part.health,part.particleCircle.center.x,part.particleCircle.center.y,part.particleCircle.radius);
    return 0;
}
