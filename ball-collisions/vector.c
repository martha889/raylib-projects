#ifndef VECTOR_C
#define VECTOR_C
#include <raylib.h>

Vector2 add_vectors(Vector2 v1, Vector2 v2) {
  Vector2 result = {v1.x + v2.x, v1.y + v2.y};
  return result;
}

Vector2 subtract_vectors(Vector2 v1, Vector2 v2) {
  Vector2 result = {v1.x - v2.x, v1.y - v2.y};
  return result;
}

Vector2 multiply_vector(Vector2 v, float scalar) {
  Vector2 result = {scalar * v.x, scalar * v.y};
  return result;
}

#endif
