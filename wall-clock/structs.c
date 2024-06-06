#ifndef STRUCTS_C
#define STRUCTS_C
#include <raylib.h>

typedef struct {
  Vector2 center;
  float radius;
  float second_hand_angle;
  Vector2 second_hand;
  float minute_hand_angle;
  Vector2 minute_hand;
  float hour_hand_angle;
  Vector2 hour_hand;
} Clock;

#endif
