#include <stdio.h>
#include <stdlib.h>

int nok(const int, const int);

int main(const int argc, const char *argv[]) {
  if (argc != 2) {
    printf("Expected number arg\n");
    return -1;
  }

  const int N = atoi(argv[1]);
  int noks[N][N];
  for (int i = 0; i < N; i++) {
    for (int j = i; j < N; j++) {
      noks[i][j] = noks[j][i] = nok(i, j);
    }
  }

  return 0;
}

int nok(const int a, const int b) {
  if (a == b) {
    return a;
  }

  if (a == 0) {
    return b;
  }

  if (b == 0) {
    return a;
  }

  if (a > b) {
    return nok(a % b, b);
  }

  return a > b ? nok(a % b, b) : nok(b % a, a);
}
