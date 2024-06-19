#include <raylib.h>
#include <stdio.h>
#include "code.c"
#include "keyboard.c"

int main() {
  srand(time(NULL));
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "consistent hashing");
  SetTargetFPS(60);

  // servers to start with
  Config* config = initialise_config(0);



  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BG_COLOR);
    draw_controls(config);
    DrawCircleLinesV(config->circle->center, config->circle->radius, WHITE);
    draw_slots(config);
    perform_keyboard_action(config);
    EndDrawing();
  }
  CloseWindow();
}
