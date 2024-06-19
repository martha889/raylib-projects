#ifndef KEYBOARD_C
#define KEYBOARD_C

#include <raylib.h>
#include <stdio.h>
#include "code.c"

#define CONTROL_POSITION_X 40;
#define CONTROL_POSITION_Y 40;

void draw_controls(Config* config) {
  char* s;
  asprintf(&s, "UP/DOWN: increase/decrease servers\nRIGHT/LEFT: increase/decrease keys");
  DrawText(s, 40, 40, 20, WHITE);
}
void increase_servers(Config* config) {
  if (config->num_servers < MAX_SERVERS) {
    add_server(config);
    // printf("Added server! Current count %d\n", config->num_servers);
  } else {
    // printf("Already at the maximum number of servers\n");
  }
}

void decrease_servers(Config* config) {
  if (config->num_servers > 0) {
    remove_server(config);
    // printf("Removed server! Current count %d\n", config->num_servers);
  } else {
    // printf("Already at zero servers!\n");
  }
}

void increase_keys(Config* config) {
    add_key(config);
}

void decrease_keys(Config* config) {
  if (config->num_keys > 0) {
    remove_key(config);
  }
}

void perform_keyboard_action(Config* config) {
  if (IsKeyPressed(KEY_UP)) {
    increase_servers(config);
  }
  if (IsKeyPressed(KEY_DOWN)) {
    decrease_servers(config);
  }
  if (IsKeyPressed(KEY_RIGHT)) {
    increase_keys(config);
  }
  if (IsKeyPressed(KEY_LEFT)) {
    decrease_keys(config);
  }
}

#endif

