#ifndef POINT_H_
#define POINT_H_

typedef struct {
  float x;
  float y;
} point;

float distance(const point *, const point *);
float rand_float();

#endif
