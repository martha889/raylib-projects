#include <raylib.h>
#include "globals.c"
#include "particle.c"
#include <stdio.h>

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "sandbox");
  // SetTargetFPS(120);

  Particle* p1 = new_particle(30, 0, +0.1, 10 + SCREEN_WIDTH * 0.25, 10 + SCREEN_HEIGHT  * 0.5);
  Particle* p2 = new_particle(10, 0, -0.1, SCREEN_WIDTH * 0.75, SCREEN_HEIGHT  * 0.5);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    // DrawFPS(40, 40);
    DrawCircle(p1->position.x, p1->position.y, p1->mass, WHITE);
    DrawCircle(p2->position.x, p2->position.y, p2->mass, WHITE);
    calculate_new_position(p1, p2);

    EndDrawing();
  }
  CloseWindow();
  return 0;
}
