#ifndef PHYSICS_C
#define PHYSICS_C

#include "object.c"
#include "vector.c"
#include "globals.c"
#include "system.c"
#include <math.h>

typedef enum {
  LEFT,
  RIGHT,
  UP,
  DOWN,
  NONE
} CollisionWall;




CollisionWall detect_collision(float x, float y, float radius) {
  if ((x + radius) >= SCREEN_WIDTH) {
    return RIGHT;
  } else if ((x - radius) < 0) {
    return LEFT;
  } else if ((y + radius) >= SCREEN_HEIGHT) {
    return DOWN;
  } else if((y - radius) < 0) {
    return UP;
  } else {
    return NONE;
  }
}

bool detect_object_collision(Object* obj1, Object* obj2) {
  // check if obj1 and obj2 collide with each other
  double distance = pow((obj1->position.x - obj2->position.x), 2) + pow((obj1->position.y - obj2->position.y), 2);

  distance = pow(distance, 0.5);
  
  if (distance <= (obj1->radius + obj2->radius)) {
    return true;
  }
  return false;
}

void act_on_object_collision(CollisionObjects* objs) {
  Object* obj1 = objs->obj1;
  Object* obj2 = objs->obj2;
  Vector2 tmp = obj1->speed;
  obj1->speed = obj2->speed;
  obj2->speed = tmp;
}

void check_all_object_collisions(Object** objects, SystemInfo* system_info) {
  for (int i=0; i<NUMBER_OF_OBJECTS-1; i++) {
    for (int j=i+1; j<NUMBER_OF_OBJECTS; j++) {
      Object* obj1 = objects[i];
      Object* obj2 = objects[j];
      CollisionObjects objs = {obj1, obj2};

      if (obj1->grid_number != obj2->grid_number) {
        continue;
      }
      if (!detect_object_collision(obj1, obj2)) {
        continue;
      }
        act_on_object_collision(&objs);
    }
  }
}


void act_on_collision(Object* obj, CollisionWall wall) {
  if ((wall == UP) || (wall == DOWN)){
    obj->speed.y = -(obj->speed.y);
  } else if ((wall == LEFT) || wall == RIGHT) {
    obj->speed.x = -(obj->speed.x);
  }
}

void update_object(Object* obj) {
  obj->speed.x += obj->acceleration.x;
  obj->speed.y += obj->acceleration.y;

  obj->position.x += obj->speed.x;
  obj->position.y += obj->speed.y;
  obj->grid_number = (int) (obj->position.y / ROW_HEIGHT) * COLS + (int) (obj->position.x / COL_WIDTH);

  CollisionWall collision_wall = detect_collision(obj->position.x, obj->position.y, obj->radius);

  if (collision_wall != NONE) {
    act_on_collision(obj, collision_wall);
  }

  obj->rotation += obj->angular_speed;
}
#endif
