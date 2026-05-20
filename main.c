#include <stdio.h>
#include <stdbool.h>


typedef struct {
  bool alive;
  int health;
  int x;
  int y;
  int radius;
} particle;

int main(void) {
    particle part;
    part.alive = true;
    part.health = 100;
    part.x = 0;
    part.y = 0;
    part.radius = 1;
    printf("particle (alive:%d | health:%d | x:%d | y:%d | radius:%d)\n", part.alive,part.health,part.x,part.y,part.radius);
    return 0;
}
