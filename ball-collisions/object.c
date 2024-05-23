#ifndef OBJECT_C
#define OBJECT_C

#include <raylib.h>
#include <stdlib.h>
#include "globals.c"

typedef struct {
  Vector2 position;
  Vector2 speed;
  Vector2 acceleration;
  // poly properties
  float rotation;
  float angular_speed;
  int sides;
  float radius;
  Color color;
  int grid_number;
} Object;

typedef struct {
  Object* obj1;
  Object* obj2;
} CollisionObjects;


Object* new_object(float pos_x, float pos_y, float speed_x, float speed_y, float acceleration_x, float acceleration_y, float angular_speed, int sides, float radius, Color color) {
  Object* obj = malloc(sizeof(Object));
  obj->position.x = pos_x;
  obj->position.y = pos_y;
  obj->speed.x = speed_x;
  obj->speed.y = speed_y;
  obj->acceleration.x = acceleration_x;
  obj->acceleration.y = acceleration_y;
  obj->angular_speed = angular_speed;
  obj->rotation = 0;
  obj->sides = sides;
  obj->radius = radius;
  obj->color = color;
  obj->grid_number = (int) (pos_y / ROW_HEIGHT) * COLS + (int) (pos_x / COL_WIDTH);
  return obj;
}

Object* new_triangle(Color color) {
  float pos_x = SCREEN_WIDTH * ((float) rand() / (float) RAND_MAX);
  float pos_y = SCREEN_HEIGHT * ((float) rand() / (float) RAND_MAX);

  float speed_x = MAX_OBJECT_SPEED * ((float) rand() / (float) RAND_MAX);
  float speed_y = MAX_OBJECT_SPEED * ((float) rand() / (float) RAND_MAX);

  float angular_speed = MAX_OBJECT_SPEED * ((float) rand() / (float) RAND_MAX);
  float radius = MAX_OBJECT_SIZE * ((float) rand() / (float) RAND_MAX);

  return new_object(pos_x, pos_y, speed_x, speed_y, 0, 0, angular_speed, 3, radius, color);
}

Object* new_polygon(Color color) {
  float pos_x = SCREEN_WIDTH * ((float) rand() / (float) RAND_MAX);
  float pos_y = SCREEN_HEIGHT * ((float) rand() / (float) RAND_MAX);

  float speed_x = MAX_OBJECT_SPEED * ((float) rand() / (float) RAND_MAX);
  float speed_y = MAX_OBJECT_SPEED * ((float) rand() / (float) RAND_MAX);

  float angular_speed = MAX_OBJECT_SPEED * ((float) rand() / (float) RAND_MAX);
  float radius = MAX_OBJECT_SIZE * ((float) rand() / (float) RAND_MAX);

  int sides = 3 + (int) ((float) (MAX_POLYGON_SIDES-2) * ((float) rand() / (float) RAND_MAX));

  return new_object(pos_x, pos_y, speed_x, speed_y, 0, 0, angular_speed, sides, radius, color);
}

Object* new_circle(Color color) {
  
  float pos_x = SCREEN_WIDTH * ((float) rand() / (float) RAND_MAX);
  float pos_y = SCREEN_HEIGHT * ((float) rand() / (float) RAND_MAX);

  float speed_x = MAX_OBJECT_SPEED * ((float) rand() / (float) RAND_MAX);
  float speed_y = MAX_OBJECT_SPEED * ((float) rand() / (float) RAND_MAX);

  float angular_speed = 0;
  float radius = MAX_OBJECT_SIZE * ((float) rand() / (float) RAND_MAX);


  return new_object(pos_x, pos_y, speed_x, speed_y, 0, 0, angular_speed, 0, radius, color);
}

#endif
