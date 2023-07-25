#include "bezier.h"
#include <math.h>

void set_curve(curve _curve, int points, SDL_Point *curve_points) {
  for (int i = 0; i < points; i++) {
    float step = (float)i / points;
    curve_points[i].x = (pow(1 - step, 3) * _curve.a.x) +
                        (3 * pow(1 - step, 3) * step * _curve.b.x) +
                        (3 * (1 - step) * pow(step, 2) * _curve.c.x) +
                        (pow(step, 3) * _curve.d.x);

    curve_points[i].y = (pow(1 - step, 3) * _curve.a.y) +
                        (3 * pow(1 - step, 3) * step * _curve.b.y) +
                        (3 * (1 - step) * pow(step, 2) * _curve.c.y) +
                        (pow(step, 3) * _curve.d.y);
  }
}
