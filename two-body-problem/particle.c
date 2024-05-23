#ifndef PARTICLE_C
#define PARTICLE_C

#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.c"

typedef struct {
  float mass;
  Vector2 position;
  Vector2 speed;
  Vector2 acceleration;
} Particle;

void print_particle(Particle* p) {
  printf("Particle:{mass: %f, position: (%f, %f), speed: (%f, %f), acceleration: (%f, %f)\n", p->mass, p->position.x, p->position.y, p->speed.x, p->speed.y, p->acceleration.x, p->acceleration.y);
}

Particle* new_particle(float mass, float speed_x, float speed_y, float pos_x, float pos_y) {
  Particle* particle = malloc(sizeof(Particle));
  particle->mass = mass;
  particle->position.x = pos_x;
  particle->position.y = pos_y;
  particle->speed.x = speed_x;
  particle->speed.y = speed_y;
  return particle;
}

float vector_distance_square(Vector2 v1, Vector2 v2) {
  return (pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
}

void calculate_new_position(Particle* p1, Particle* p2) {
  float acceleration = (GRAVITIONAL_CONSTANT * p2->mass) / vector_distance_square(p1->position, p2->position);
  p1->acceleration.x = acceleration * (p2->position.x - p1->position.x) / pow((vector_distance_square(p1->position, p2->position)), 0.5);
  p1->acceleration.y = acceleration * (p2->position.y - p1->position.y) / pow((vector_distance_square(p1->position, p2->position)), 0.5);
  p2->acceleration.x = acceleration * (p1->position.x - p2->position.x) / pow((vector_distance_square(p1->position, p2->position)), 0.5);
  p2->acceleration.y = acceleration * (p1->position.y - p2->position.y) / pow((vector_distance_square(p1->position, p2->position)), 0.5);

  p1->speed.x += p1->acceleration.x;
  p1->speed.y += p1->acceleration.y;
  p2->speed.x += p2->acceleration.x;
  p2->speed.y += p2->acceleration.y;

  p1->position.x += p1->speed.x;
  p1->position.y += p1->speed.y;
  p2->position.x += p2->speed.x;
  p2->position.y += p2->speed.y;

}

#endif
