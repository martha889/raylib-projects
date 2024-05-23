#include <raylib.h>
#include <stdio.h>
#include <pthread.h>

#include "object.c"
#include "system.c"
#include "physics.c"
#include "globals.c"
#include "color.c"

int count_objects = 0;


Object** init_objects() {
  Object** objects = malloc(sizeof(Object*) * NUMBER_OF_OBJECTS);
  for (int i=0; i<NUMBER_OF_OBJECTS; i++) {
    objects[i] = NULL;
  }
  return objects;
}

void update_all_objects(Object** objs) {
  for (int i=0; i<NUMBER_OF_OBJECTS; i++) {
    if (objs[i] != NULL) {
      update_object(objs[i]);
    } else {
      break;
    }
  }
  
}

void register_object(Object** objs, Object* obj) {
  if (count_objects >= NUMBER_OF_OBJECTS) {
    printf("Error: reached the maximum number of objects\n");
    return;
  }

  objs[count_objects] = obj;
  count_objects += 1;
}

void render_objects(Object** objs, SystemInfo* system_info) {
  check_all_object_collisions(objs, system_info);
  for (int i=0; i<count_objects; i++) {
    Object* obj = objs[i];
    // DrawPoly(obj->position, obj->sides, obj->radius, obj->rotation, obj->color);
    DrawCircle(obj->position.x, obj->position.y, obj->radius, obj->color);
  }
}



int main() {
  Object** objects = init_objects();
  SystemInfo* system_info = init_system_info();

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "sandbox");
  SetTargetFPS(120);

  for (int i=0; i<NUMBER_OF_OBJECTS; i++) {
    Object* poly = new_polygon(get_random_color());
    register_object(objects, poly);
  }

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    // DrawFPS(40, 40);

    render_objects(objects, system_info);
    update_all_objects(objects);


    EndDrawing();
  }
  CloseWindow();
  return 0;
}
