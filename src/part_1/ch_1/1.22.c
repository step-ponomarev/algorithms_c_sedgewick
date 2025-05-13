#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define max(a, b) (a > b ? a : b)

// use src/lib/graph_link_quck_union
int main(const int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Illegal argument size: %d", argc);
    return -1;
  }

  const int max_val = atoi(argv[1]);
  int id[max_val];
  int sz[max_val];

  struct timeval start, end;
  gettimeofday(&start, NULL); // Начало замера
  for (int i = 0; i < max_val; i++) {
    id[i] = i;
    sz[i] = 1;
  }

  int union_count = 0;
  int p, q;
  while (scanf("%d %d\\n", &p, &q) == 2) {
    if (p < 0 || p >= max_val || q < 0 || q >= max_val) {
      fprintf(stderr, "Values must be < %d, p: %d, q: %d\n", max_val, p, q);
      return -1;
    }

    // one root -> one union

    int i, j;
    for (i = p; i != id[i]; i = id[i]) {
      id[i] = id[id[i]];
    }

    for (j = q; j != id[j]; j = id[j]) {
      id[j] = id[id[j]];
    }

    if (i == j) {
      continue;
    }

    if (sz[i] < sz[j]) { // присоеденяем к большему меньший
      id[i] = id[j];
      sz[j] += sz[i];

      union_count = max(sz[j], union_count);
    } else {
      id[j] = id[i];
      sz[i] += sz[j];

      union_count = max(sz[i], union_count);
    }

    printf("%d-%d\n", p, q);
    if (union_count == max_val - 1) {
      break;
    }
  }
  gettimeofday(&end, NULL); // Конец замера

  // Расчет времени в миллисекундах
  long seconds = end.tv_sec - start.tv_sec;
  long micros = ((seconds * 1000000) + end.tv_usec) - start.tv_usec;
  double millis = micros / 1000.0;

  printf("Took: %.2f ms\n", millis);

  return 0;
}
