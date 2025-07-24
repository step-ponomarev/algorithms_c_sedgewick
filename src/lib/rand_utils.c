#include "rand_utils.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

bool init = false;

int random_int(int from, int to) {
  if (!init) {
    srand(time(NULL));
    init = true;
  }

  if (from < 0 || from >= to) {
    return -1;
  }

  return from + (rand() % (to - from));
}
