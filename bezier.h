#include<SDL2/SDL.h>
#include "main.h"

typedef struct {
  vector_2 a;
  vector_2 b;
  vector_2 c;
  vector_2 d;
} curve;


void set_curve(curve _curve, int points, SDL_Point *curve_points);

