#include "rand_utils.h"
#include <stdlib.h>

int random_int(int from, int to) {
  if (from < 0 || from >= to) {
    return -1;
  }

  return from + (rand() % (to - from));
}
