
#ifndef HEADER_STOP
#define HEADER_STOP
#include "main.h"
#include <SDL2/SDL.h>
typedef struct {
  vector_2 a;
  vector_2 b;
  vector_2 c;
  vector_2 d;
} curve;
typedef struct {
  vector_2 orig_pos;
  curve tentacle_pos;
  int distance;
  int pulling;
} tentacle;

void set_curve(curve _curve, int points, SDL_Point *curve_points);
#endif
