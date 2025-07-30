#include "rand_utils.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

int random_int(int from, int to) {
  static bool init;
  if (!init) {
    srand(time(NULL));
    init = true;
  }

  if (from < 0 || from >= to) {
    return -1;
  }

  return from + (rand() % (to - from));
}
