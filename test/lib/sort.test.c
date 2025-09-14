// geerated file
//
#include "../../src/lib/sort.h"
#include "../../src/collections/comparator.h"

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*** ========= Регулируемые параметры (можно переопределять -D ...) =========
 * ***/
#ifndef DEFAULT_N
#define DEFAULT_N                                                              \
  (200000u) // по умолчанию 200k (для Shell ок; для вставок тоже терпимо)
#endif
#ifndef DEFAULT_TRIALS
#define DEFAULT_TRIALS 3 // число прогонов
#endif
#ifndef WARMUP_TRIALS
#define WARMUP_TRIALS 0 // тёплый прогон без учёта времени
#endif
#ifndef VALUE_RANGE
#define VALUE_RANGE 2000001 // значения в диапазоне [-R/2, R/2]
#endif
#ifndef PRINT_SAMPLE
#define PRINT_SAMPLE 0 // печатать первые 20 значений до/после
#endif
#ifndef CHECK_SORTED
#define CHECK_SORTED 1 // валидировать отсортированность
#endif
#ifndef STACK_BUF_LIMIT
#define STACK_BUF_LIMIT                                                        \
  8192 // не относится к бенчу, но вдруг пригодится в сортировке
#endif
/*** =========================================================================
 * ***/

static uint64_t now_ns(void) {
#if defined(CLOCK_MONOTONIC)
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec;
#else
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (uint64_t)tv.tv_sec * 1000000000ull + (uint64_t)tv.tv_usec * 1000ull;
#endif
}

// xorshift64*
static inline uint64_t xorshift64star(uint64_t *s) {
  uint64_t x = *s ? *s : 0x9e3779b97f4a7c15ULL;
  x ^= x >> 12;
  x ^= x << 25;
  x ^= x >> 27;
  *s = x;
  return x * 2685821657736338717ULL;
}

static void fill_random_ints(int *a, uint32_t n, uint64_t seed) {
  uint64_t s = seed;
  for (uint32_t i = 0; i < n; i++) {
    uint64_t r = xorshift64star(&s);
    a[i] = (int)(r % VALUE_RANGE) - (VALUE_RANGE / 2);
  }
}

static int is_sorted_ints(const int *a, uint32_t n) {
#if CHECK_SORTED
  for (uint32_t i = 1; i < n; i++)
    if (a[i - 1] > a[i])
      return 0;
#endif
  return 1;
}

static void humanize(double ms, char *out, size_t out_sz) {
  if (ms < 1.0)
    snprintf(out, out_sz, "%.3f ms", ms);
  else if (ms < 1000.0)
    snprintf(out, out_sz, "%.2f ms", ms);
  else
    snprintf(out, out_sz, "%.2f s", ms / 1000.0);
}

static void print_sample(const char *label, const int *a, uint32_t n) {
#if PRINT_SAMPLE
  printf("%s:", label);
  uint32_t k = n < 20 ? n : 20;
  for (uint32_t i = 0; i < k; i++)
    printf(" %d", a[i]);
  if (n > k)
    printf(" ...");
  printf("\n");
#else
  (void)label;
  (void)a;
  (void)n;
#endif
}

int main(int argc, char **argv) {
  uint32_t n = (argc > 1) ? (uint32_t)strtoul(argv[1], NULL, 10) : DEFAULT_N;
  int trials = (argc > 2) ? atoi(argv[2]) : DEFAULT_TRIALS;
  uint64_t seed = (argc > 3) ? strtoull(argv[3], NULL, 10)
                             : (uint64_t)time(NULL) ^ 0x9e3779b97f4a7c15ULL;

  if (n == 0) {
    fprintf(stderr, "n must be > 0\n");
    return 1;
  }
  if (trials <= 0)
    trials = 1;

  printf("Benchmark sort() on %u ints | trials=%d | seed=%" PRIu64 "\n", n,
         trials, seed);

  size_t bytes = (size_t)n * sizeof(int);
  int *data = (int *)malloc(bytes);
  int *work = (int *)malloc(bytes);
  if (!data || !work) {
    fprintf(stderr, "Allocation failed (%zu bytes)\n", bytes);
    free(data);
    free(work);
    return 1;
  }

  // warmup
  for (int w = 0; w < WARMUP_TRIALS; w++) {
    fill_random_ints(data, n, seed + w);
    memcpy(work, data, bytes);
    sort(work, 0, n, &compare_int, sizeof(int));
  }

  double best_ms = 1e300, sum_ms = 0.0;

  for (int t = 1; t <= trials; t++) {
    fill_random_ints(data, n, seed + (uint64_t)t * 0x9e3779b97f4a7c15ULL);
    memcpy(work, data, bytes);

    print_sample("before", work, n);

    uint64_t t0 = now_ns();
    sort(work, 0, n, &compare_int, sizeof(int)); // твоя сортировка: [0, n)
    uint64_t t1 = now_ns();

    double ms = (t1 - t0) / 1e6;
    sum_ms += ms;
    if (ms < best_ms)
      best_ms = ms;

    if (!is_sorted_ints(work, n)) {
      fprintf(stderr, "[trial %d] NOT SORTED!\n", t);
      free(data);
      free(work);
      return 2;
    }

    print_sample("after ", work, n);

    char buf[64];
    humanize(ms, buf, sizeof buf);
    printf("[trial %d] elapsed: %s\n", t, buf);
  }

  char bbuf[64], abuf[64];
  humanize(best_ms, bbuf, sizeof bbuf);
  humanize(sum_ms / trials, abuf, sizeof abuf);
  printf("Best: %s | Avg: %s\n", bbuf, abuf);

  free(data);
  free(work);
  return 0;
}
