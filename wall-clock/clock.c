#ifndef CLOCK_C
#define CLOCK_C

#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "structs.c"
#include "trigonometry.c"

float SECOND_INCREMENT = 2 * M_PI / 60;
float MINUTE_INCREMENT = 2 * M_PI / (60 * 60);
float HOUR_INCREMENT = 2 * M_PI / (60 * 60 * 12);

void debug_clock(Clock* clock) {
  printf("center (%f, %f)\n", clock->center.x, clock->center.y);
  printf("radius %f\n", clock->radius);
  printf("angles s[%f], m[%f], h[%f]\n", clock->second_hand_angle, clock->minute_hand_angle, clock->hour_hand_angle);
  printf("coords s(%f, %f), m(%f, %f), h(%f, %f)\n", clock->second_hand.x, clock->second_hand.y, clock->minute_hand.x, clock->minute_hand.y, clock->hour_hand.x, clock->hour_hand.y);
}

struct tm* get_current_time() {
  // Get the current time
  time_t current_time = time(NULL);

  // Check if getting the time was successful
  if (current_time == -1) {
    perror("time");
  }

  // Convert it to local time representation
  struct tm *local_time = localtime(&current_time);

  // Check if conversion was successful
  if (local_time == NULL) {
    perror("localtime");
  }

  return local_time;
}

void set_angles(struct tm* current_time, Clock* clock) {
  clock->second_hand_angle = -M_PI_2 + current_time->tm_sec * SECOND_INCREMENT;
  clock->minute_hand_angle = -M_PI_2 + (current_time->tm_min * 60 + current_time->tm_sec) * MINUTE_INCREMENT;
  clock->hour_hand_angle = -M_PI_2 + ((current_time->tm_hour % 12) * 60 * 60 + current_time->tm_min * 60 + current_time->tm_sec) * HOUR_INCREMENT;
  
}

Clock* initialize_clock(int screen_height, int screen_width, float radius) {
  struct tm* current_time = get_current_time();
  Clock* new_clock = malloc(sizeof(Clock));
  new_clock->center.x = (float) screen_width / 2;
  new_clock->center.y = (float) screen_height / 2;
  new_clock->radius = radius;
  set_angles(current_time, new_clock);
  update_clock_second(new_clock);
  update_clock_minute(new_clock);
  update_clock_hour(new_clock);

  return new_clock;
}

// limit FPS to 1
void update_clock(Clock* clock) {
  clock->second_hand_angle += SECOND_INCREMENT;
  clock->minute_hand_angle += MINUTE_INCREMENT;
  clock->hour_hand_angle += HOUR_INCREMENT;

  // prevent overflow
  if (clock->second_hand_angle > 2 * M_PI) {
    clock->second_hand_angle -= 2 * M_PI;
  }

  if (clock->minute_hand_angle > 2 * M_PI) {
    clock->minute_hand_angle -= 2 * M_PI;
  }

  if (clock->hour_hand_angle > 2 * M_PI) {
    clock->hour_hand_angle -= 2 * M_PI;
  }

  update_clock_second(clock);
  update_clock_minute(clock);
  update_clock_hour(clock);
 
}

#endif
