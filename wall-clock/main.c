#include <raylib.h>
#include <stdio.h>
#include "globals.c"
#include "clock.c"

// screen
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 800;

float CLOCK_RADIUS = 300;

void draw_clock(Clock* clock) {
  // draw second hand
  DrawLineV(clock->center, clock->second_hand, RED);
  // draw minute hand
  DrawLineV(clock->center, clock->minute_hand, YELLOW);
  // draw hour hand
  DrawLineV(clock->center, clock->hour_hand, BLUE);
}

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Clock");
  SetTargetFPS(1);
  Clock* clock = initialize_clock(SCREEN_HEIGHT, SCREEN_WIDTH, CLOCK_RADIUS);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BG_COLOR);
    DrawCircleLinesV(clock->center, CLOCK_RADIUS, WHITE);
    draw_clock(clock);
    update_clock(clock);
    EndDrawing();
  }
  CloseWindow();
}
