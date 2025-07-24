#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a > b ? b : a)

int main(const int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Expected 2 args, got %d\n", argc);
  }

  int a = atoi(argv[1]);
  int b = atoi(argv[2]);

  int res = 0;
  while (1) {
    int max = MAX(a, b);
    int min = MIN(a, b);

    a = min;
    if ((b = max % min) == 0) {
      res = min;
      break;
    }
  }

  fprintf(stdout, "Res: %d\n", res);

  return 0;
}
