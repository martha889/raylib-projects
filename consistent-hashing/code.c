#ifndef CODE_C
#define CODE_C
#include <raylib.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// screen
int MAX_SERVERS = 10;

#define MAX_KEYS_PER_SERVER 10
#define MAX_KEY_SIZE 10
#define BG_COLOR BLACK
#define GLOBAL_CIRCLE_RADIUS (SCREEN_WIDTH + SCREEN_HEIGHT) / 6
#define SMALL_CIRCLE_RADIUS (GLOBAL_CIRCLE_RADIUS / (MAX_SERVERS))
#define DISTANCE_FACTOR 100

#define KEY_FONT_SIZE 10
#define KEY_COLOR WHITE
#define KEY_MOVEMENT_DIFF 0.01

float SCREEN_WIDTH = 800;
float SCREEN_HEIGHT = 800;

typedef enum {
  OBJECT_EMPTY = 1,
  OBJECT_SERVER = 2,
} ObjectType;

typedef struct {
  Vector2 center;
  float radius;
} Circle;

typedef struct {
  char* text;
  Vector2 src;
  Vector2 dest;
  Vector2 current;
  bool reached;
} Key;

typedef struct {
  ObjectType object;
  Key* keys; // array of strings
  Vector2 coords;
  int num_keys;
} Slot;

typedef struct {
  int num_servers;
  int num_keys; // only used for naming keys in increasing order;
  Slot* slots;
  Circle* circle;
} Config;

float get_random_number() {
  float number = (double) rand() / RAND_MAX;
  return number;
}

float max(float x, float y) {
  if (x >= y) {
    return x;
  }
  return y;
}

float min(float x, float y) {
  if (x <= y) {
    return x;
  }
  return y;
}

double distance_between_two_points(Vector2* p1, Vector2* p2) {
  double term1 = pow((double) p1->x - (double) p2->x, 2) / DISTANCE_FACTOR;
  double term2 = pow((double) p1->y - (double) p2->y, 2) / DISTANCE_FACTOR;
  return term1 + term2;
}

Circle* initialise_circle(int x, int y, float r) {
  Circle* circle = malloc(sizeof(Circle));
  circle->center.x = x;
  circle->center.y = y;
  circle->radius = r;
  return circle;
}

// f = 0 -> upwards; f = 0.25 -> facing right, ...
Vector2 polar_to_cartesian(Vector2 center, float radius, float fraction) {
  float x_coord = center.x + radius * sinf(fraction * 2 * M_PI);
  float y_coord = center.y + radius * cosf(fraction * 2 * M_PI);

  float tan_theta = (y_coord - center.y) / (x_coord - center.x);
  float theta = atanf(tan_theta);
  float theta_degrees = theta * 180 / M_PI;

  // printf("Angle in degrees %f\n", theta_degrees);
  
  return (Vector2) {(float) x_coord, (float) y_coord};
  
}

// initialise and return the array of strings that holds the keys
Key* initialise_keys() {
  Key* keys = malloc(sizeof(Key) * MAX_KEYS_PER_SERVER);
  for (int i=0; i<MAX_KEYS_PER_SERVER; i++) {
    keys[i].text = malloc(MAX_KEY_SIZE);
    keys[i].reached = false;
  }
  return keys;
}

Slot* initialise_slots(Config* config) {
  Slot* slots = malloc(sizeof(Slot) * MAX_SERVERS);
  float diff = (float) 1 / MAX_SERVERS;
  float fraction = 0;
  for (int i=0; i<MAX_SERVERS; i++) {
    slots[i].object = OBJECT_EMPTY;
    slots[i].keys = initialise_keys();
    slots[i].coords = polar_to_cartesian(config->circle->center, config->circle->radius, fraction);
    slots[i].num_keys = 0;
    fraction += diff;
  }
  return slots;
}


Config* initialise_config(int num_servers) {
  Config* config = malloc(sizeof(Config));
  int center_x = (int) SCREEN_WIDTH / 2;
  int center_y = (int) SCREEN_HEIGHT / 2;
  config->circle = initialise_circle(center_x, center_y, GLOBAL_CIRCLE_RADIUS);
  config->num_servers = num_servers;
  config->num_keys = 0;
  config->slots = initialise_slots(config);
  return config;
}

// move text from key's source to key's destination
void move_key_text_old(Key* key) {
  DrawText(key->text, (int) key->current.x, (int) key->current.y, KEY_FONT_SIZE, KEY_COLOR);
  float slope = (key->dest.y - key->src.y) / (key->dest.x - key->src.x);
  float intercept = key->src.y - slope * key->src.x;
  int sign = 1;

  if ((key->dest.x - key->src.x) < 0) {
    sign *= -1;
  }

  key->current.x += KEY_MOVEMENT_DIFF * sign;
  key->current.y = key->current.x * slope + intercept;

  if (key->current.x < min(key->src.x, key->dest.x) || key->current.x > max(key->src.x, key->dest.x)) {
    key->reached = true;
  }

}

void move_key_text(Key* key) {
  DrawText(key->text, (int) key->current.x, (int) key->current.y, KEY_FONT_SIZE, KEY_COLOR);
  float dx = key->dest.x - key->src.x;
  float dy = key->dest.y - key->src.y;

  double old_distance =distance_between_two_points(&key->current, &key->dest);

  key->current.x += dx * KEY_MOVEMENT_DIFF;
  key->current.y += dy * KEY_MOVEMENT_DIFF;

  double new_distance =distance_between_two_points(&key->current, &key->dest);

  // printf("Key: %s, (old, new): (%f, %f)\n", key->text, old_distance, new_distance);

  if (new_distance >= old_distance) {
    key->reached = true;
  }
}
void draw_slots(Config* config) {
  for (int i=0; i<MAX_SERVERS; i++) {
    Slot* slot = &config->slots[i];
    if (slot->object == OBJECT_SERVER) {
      DrawCircleV(slot->coords, SMALL_CIRCLE_RADIUS, RED);
      // +2 to accomodate \n,\0 at the end of each string
      char* text_to_draw = malloc(sizeof(char) * MAX_KEYS_PER_SERVER * (MAX_KEY_SIZE+2));
      for (int j=0; j<slot->num_keys; j++) {
        Key* key = &slot->keys[j];
        if (key->reached) {
          strcat(text_to_draw, slot->keys[j].text);
        } else {
          move_key_text(key);
        }
      }
      DrawText(text_to_draw, slot->coords.x, slot->coords.y, 10, WHITE);
    }
  }
}

void redistribute_key(Config* config, Key* key, int slot_number) {
  if (config->num_servers == 0) {
    printf("Can't redistribute key as there are no servers!\n");
    return;
  }

  if (config->num_keys == MAX_KEYS_PER_SERVER * config->num_servers) {
    printf("All Servers at maximum capacity, can't redistribute!");
    return;
  }

  printf("redistributing key %s...\n", key->text);

  slot_number += 1;
  slot_number %= MAX_SERVERS;

  int count = MAX_KEYS_PER_SERVER * MAX_SERVERS;
  while (config->slots[slot_number].object == OBJECT_EMPTY || config->slots[slot_number].num_keys >= MAX_KEYS_PER_SERVER) {
    slot_number += 1;
    slot_number %= MAX_SERVERS;
    count -= 1;
    if (count == 0) {
      printf("Can't redistribute keys!\n");
      return;
    }
  }

  Slot* new_slot = &config->slots[slot_number];
  key->src = key->current;
  key->dest = new_slot->coords;
  key->reached = false;
  memcpy(&new_slot->keys[new_slot->num_keys], key, sizeof(Key));
  new_slot->num_keys += 1;
  config->num_keys += 1;
}

void add_server(Config* config) {
  // figure out the slot in which the server will go
  // assign it to that slot
  // add key
  int slot_number = (int) (get_random_number() * MAX_SERVERS);

  while (config->slots[slot_number].object == OBJECT_SERVER && config->num_servers < MAX_SERVERS) {
    slot_number = (int) (get_random_number() * MAX_SERVERS);
  }
  // printf("Added server to slot number: %d\n", slot_number);
  config->slots[slot_number].object = OBJECT_SERVER; 
  config->num_servers += 1;
}

void remove_server(Config* config) {
  // which server to remove?
  // for now, remove the first server we encounter
  // TODO: give user the option
  // TODO: add logic to redistribute keys on server removal

  for (int i=0; i<MAX_SERVERS; i++) {
    if (config->slots[i].object == OBJECT_SERVER && config->num_servers > 0) {
      // printf("Removed server from slot number: %d\n", i);
      Slot* slot = &config->slots[i];
      config->slots[i].object =OBJECT_EMPTY;
      config->num_servers -= 1;

      for (int j=0; j<slot->num_keys; j++) {
        redistribute_key(config, &slot->keys[j], i);
      }
      slot->num_keys = 0;
      break;
    }
  }
}

void add_key(Config* config) {
  int slot_number = (int) (get_random_number() * MAX_SERVERS);

  if (config->num_servers == 0) {
    printf("No servers available to add key to!\n");
    return;
  }

  while (config->slots[slot_number].object == OBJECT_EMPTY) {
    slot_number += 1;
    slot_number %= MAX_SERVERS;
  }

  Slot* slot = &config->slots[slot_number];
  Key* key = &slot->keys[slot->num_keys];
  key->src = config->circle->center;
  key->current = config->circle->center;
  key->dest = slot->coords;
  key->reached = false;
  asprintf(&key->text, "key%d\n", config->num_keys);

  slot->num_keys += 1;
  config->num_keys += 1;
}

void remove_key(Config* config) {
  // which key to remove?
  // for now, remove the last key from a server.
  // TODO: give user the option to remove key
  
  int slot_number = (int) (get_random_number() * MAX_SERVERS);

  Slot* slot = &config->slots[slot_number];
  Key* key_to_redistribute = &slot->keys[slot->num_keys];
  redistribute_key(config, key_to_redistribute, slot_number);
  slot->num_keys -= 1;
  config->num_keys -= 1;
}

#endif
