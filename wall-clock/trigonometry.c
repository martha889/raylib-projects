#ifndef TRIGONOMETRY_C
#define TRIGONOMETRY_C

#include <math.h>
#include <raylib.h>
#include <stdlib.h>
#include "structs.c"

float SECOND_FACTOR = 0.9;
float MINUTE_FACTOR = 0.6;
float HOUR_FACTOR = 0.3;

void update_clock_second(Clock* clock) {
  float radius = SECOND_FACTOR * clock->radius;
  clock->second_hand.x = clock->center.x + radius * cosf(clock->second_hand_angle);
  clock->second_hand.y = clock->center.y + radius * sinf(clock->second_hand_angle);
}

void update_clock_minute(Clock* clock) {
  float radius = MINUTE_FACTOR * clock->radius;
  clock->minute_hand.x = clock->center.x + radius * cosf(clock->minute_hand_angle);
  clock->minute_hand.y = clock->center.y + radius * sinf(clock->minute_hand_angle);
}

void update_clock_hour(Clock* clock) {
  float radius = HOUR_FACTOR * clock->radius;
  clock->hour_hand.x = clock->center.x + radius * cosf(clock->hour_hand_angle);
  clock->hour_hand.y = clock->center.y + radius * sinf(clock->hour_hand_angle);
}


#endif
