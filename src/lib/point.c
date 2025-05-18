#include "point.h"
#include <math.h>
#include <stdlib.h>

float distance(const point *p1, const point *p2) {
  if (!p1 || !p2) {
    return 0.0f;
  }

  const float dx = p1->x - p2->x;
  const float dy = p1->y - p2->y;
  return sqrtf(dx * dx + dy * dy);
}

float rand_float() { return (float)rand() / RAND_MAX; }
