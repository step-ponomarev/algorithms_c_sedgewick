#include "comparator.h"

char compare_int(const void *a, const void *b) {
  int int_a = *(int *)(a);
  int int_b = *(int *)(b);

  return (int_a > int_b) - (int_a < int_b);
}
