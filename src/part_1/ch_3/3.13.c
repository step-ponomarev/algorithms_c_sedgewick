#include <stdio.h>
#include <stdlib.h>

int main(const int argc, const char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Pass one int arg");
    return -1;
  }

  const int N = atoi(argv[1]);
  int vals[N];
  int amount = N - 2; //-1 -0

  for (int i = 2; i < N; vals[i++] = 1)
    ;

  for (int i = 2; i < N; i++) {
    if (vals[i] == 0) {
      continue;
    }

    for (int j = i, p = j * i; p < N && p > 0; p = ++j * i) {
      if (!vals[p]) {
        continue;
      }

      amount--;
      vals[p] = 0;
    }
  }

  printf("%d prime numbers < %d\n", amount, N);

  return 0;
}
