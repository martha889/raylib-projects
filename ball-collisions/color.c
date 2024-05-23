#ifndef COLOR_C
#define COLOR_C

#include <raylib.h>
#include <stdlib.h>

Color colors[] = {LIGHTGRAY, GRAY, DARKGRAY, YELLOW, 
                  GOLD, ORANGE, PINK, RED, MAROON,
                  GREEN, LIME, DARKGREEN, SKYBLUE,
                  BLUE, DARKBLUE, PURPLE, VIOLET,
                  DARKPURPLE, BEIGE, BROWN, DARKBROWN,
                  WHITE, BLANK, MAGENTA, RAYWHITE};

unsigned int color_size = (int) sizeof(colors) / sizeof(Color);

Color get_random_color() {
  int color = (int) color_size * ((float) rand() / (float) RAND_MAX);

  color = color % color_size;

  return colors[color];
}
#endif
