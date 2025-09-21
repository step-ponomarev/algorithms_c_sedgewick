// test/lib/sort_bench.c
// Бенч сортировки: паттерны данных + понятный вывод
#include "../../src/lib/sort.h"
#include "../../src/collections/comparator.h"

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*** ========== Конфиг (можно переопределять -D ...) =======================
 * ***/
#ifndef DEFAULT_N
#define DEFAULT_N (200000u)
#endif
#ifndef DEFAULT_TRIALS
#define DEFAULT_TRIALS 3
#endif
#ifndef WARMUP_TRIALS
#define WARMUP_TRIALS 0
#endif
#ifndef VALUE_RANGE
#define VALUE_RANGE 2000001 // для RANDOM
#endif
#ifndef PRINT_SAMPLE
#define PRINT_SAMPLE 0
#endif
#ifndef CHECK_SORTED
#define CHECK_SORTED 1
#endif

// Паттерны данных
#ifndef DEFAULT_PATTERN
#define DEFAULT_PATTERN 0 // 0=random,1=almost,2=block,3=stride,4=dups
#endif
#ifndef DEFAULT_ALMOST_FRAC
#define DEFAULT_ALMOST_FRAC 0.01 // 1% случайных свапов после сортировки
#endif
#ifndef DEFAULT_BLOCK_SIZE
#define DEFAULT_BLOCK_SIZE 256 // размер блока для перетасовки
#endif
#ifndef DEFAULT_STRIDE_K
#define DEFAULT_STRIDE_K 64 // k-классы по модулю K
#endif
#ifndef DEFAULT_DUP_RANGE
#define DEFAULT_DUP_RANGE 1024 // узкий диапазон для дублей
#endif
/*** =======================================================================
 * ***/

typedef enum {
  PAT_RANDOM = 0,
  PAT_ALMOST = 1,
  PAT_BLOCK = 2,
  PAT_STRIDE = 3,
  PAT_DUPS = 4
} Pattern;

// монотонные часы
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

static int is_sorted_ints(const int *a, uint32_t n) {
#if CHECK_SORTED
  for (uint32_t i = 1; i < n; i++)
    if (a[i - 1] > a[i])
      return 0;
#endif
  return 1;
}

// ===== генераторы =====
static void fill_random_ints(int *a, uint32_t n, uint64_t seed, int range) {
  uint64_t s = seed;
  for (uint32_t i = 0; i < n; i++) {
    uint64_t r = xorshift64star(&s);
    a[i] = (int)(r % (uint64_t)range) - (range / 2);
  }
}

static int cmp_int_qsort(const void *pa, const void *pb) {
  int a = *(const int *)pa, b = *(const int *)pb;
  return (a > b) - (a < b);
}

static void make_almost_sorted(int *a, uint32_t n, uint64_t seed, double frac) {
  // начнём со случайных чисел, но отсортируем «идеально»
  // используем qsort, чтобы не «подсмотреть» нашу sort()
  qsort(a, n, sizeof(int), cmp_int_qsort);
  if (frac <= 0.0)
    return;
  uint64_t s = seed ^ 0xD1CEFACEB00BULL;
  uint32_t swaps = (uint32_t)(frac * (double)n);
  for (uint32_t k = 0; k < swaps; k++) {
    uint32_t i = (uint32_t)(xorshift64star(&s) % n);
    uint32_t j = (uint32_t)(xorshift64star(&s) % n);
    int t = a[i];
    a[i] = a[j];
    a[j] = t;
  }
}

static void fisher_yates_block(int *a, uint32_t start, uint32_t len,
                               uint64_t *s) {
  for (uint32_t u = len; u > 1; --u) {
    uint32_t j = (uint32_t)(xorshift64star(s) % u);
    int t = a[start + u - 1];
    a[start + u - 1] = a[start + j];
    a[start + j] = t;
  }
}

static void make_blockwise_shuffled(int *a, uint32_t n, uint64_t seed,
                                    uint32_t B) {
  qsort(a, n, sizeof(int), cmp_int_qsort);
  if (B == 0)
    return;
  uint64_t s = seed ^ 0xABCDEF1234567890ULL;
  for (uint32_t start = 0; start < n; start += B) {
    uint32_t len = (start + B <= n) ? B : (n - start);
    fisher_yates_block(a, start, len, &s);
  }
}

static void make_stride_shuffled(int *a, uint32_t n, uint64_t seed,
                                 uint32_t K) {
  qsort(a, n, sizeof(int), cmp_int_qsort);
  if (K <= 1)
    return;
  uint64_t s = seed ^ 0xFEEDFACECAFEBEEFULL;
  for (uint32_t r = 0; r < K; r++) {
    // сделаем cnt обменов внутри класса r mod K
    uint32_t cnt = 0;
    for (uint32_t i = r; i < n; i += K)
      cnt++;
    uint32_t swaps = cnt;
    for (uint32_t t = 0; t < swaps; t++) {
      uint32_t ai = r + K * (uint32_t)(xorshift64star(&s) % cnt);
      uint32_t bi = r + K * (uint32_t)(xorshift64star(&s) % cnt);
      int tmp = a[ai];
      a[ai] = a[bi];
      a[bi] = tmp;
    }
  }
}

static void make_many_duplicates(int *a, uint32_t n, uint64_t seed, int range) {
  if (range < 2)
    range = 2;
  fill_random_ints(a, n, seed, range);
}

// ===== счётчик сравнений (опционально) =====
#if COUNT_COMPARISONS
static volatile uint64_t g_cmp_count = 0;
static int compare_int_counting(const void *a, const void *b) {
  g_cmp_count++;
  return compare_int(a, b);
}
#define CMP_PTR (&compare_int_counting)
#else
#define CMP_PTR (&compare_int)
#endif

static const char *pattern_name(Pattern p) {
  switch (p) {
  case PAT_RANDOM:
    return "RANDOM";
  case PAT_ALMOST:
    return "ALMOST_SORTED";
  case PAT_BLOCK:
    return "BLOCKWISE_SHUFFLED";
  case PAT_STRIDE:
    return "STRIDE_SHUFFLED";
  case PAT_DUPS:
    return "DUPLICATES";
  default:
    return "UNKNOWN";
  }
}

static void describe_pattern(Pattern p, double param, char *out, size_t n) {
  switch (p) {
  case PAT_RANDOM:
    snprintf(out, n, "pattern=%s (range=%d)", pattern_name(p), VALUE_RANGE);
    break;
  case PAT_ALMOST:
    snprintf(out, n, "pattern=%s (frac=%.2f%%)", pattern_name(p),
             100.0 * param);
    break;
  case PAT_BLOCK:
    snprintf(out, n, "pattern=%s (block_size=%u)", pattern_name(p),
             (unsigned)param);
    break;
  case PAT_STRIDE:
    snprintf(out, n, "pattern=%s (K=%u)", pattern_name(p), (unsigned)param);
    break;
  case PAT_DUPS:
    snprintf(out, n, "pattern=%s (dup_range=%u)", pattern_name(p),
             (unsigned)param);
    break;
  default:
    snprintf(out, n, "pattern=%s", pattern_name(p));
  }
}

int main(int argc, char **argv) {
  uint32_t n = (argc > 1) ? (uint32_t)strtoul(argv[1], NULL, 10) : DEFAULT_N;
  int trials = (argc > 2) ? atoi(argv[2]) : DEFAULT_TRIALS;
  uint64_t seed = (argc > 3) ? strtoull(argv[3], NULL, 10)
                             : (uint64_t)time(NULL) ^ 0x9e3779b97f4a7c15ULL;
  Pattern pattern = (argc > 4) ? (Pattern)strtoul(argv[4], NULL, 10)
                               : (Pattern)DEFAULT_PATTERN;

  // параметр паттерна (смысл зависит от pattern)
  double param = 0.0;
  if (argc > 5) {
    param = atof(argv[5]);
  } else {
    switch (pattern) {
    case PAT_ALMOST:
      param = DEFAULT_ALMOST_FRAC;
      break;
    case PAT_BLOCK:
      param = DEFAULT_BLOCK_SIZE;
      break;
    case PAT_STRIDE:
      param = DEFAULT_STRIDE_K;
      break;
    case PAT_DUPS:
      param = DEFAULT_DUP_RANGE;
      break;
    default:
      param = 0.0;
    }
  }

  if (n == 0) {
    fprintf(stderr, "n must be > 0\n");
    return 1;
  }
  if (trials <= 0)
    trials = 1;

  char patbuf[128];
  describe_pattern(pattern, param, patbuf, sizeof patbuf);

  printf("Benchmark sort() on %u ints | trials=%d | seed=%" PRIu64 "\n", n,
         trials, seed);
  printf("Dataset: type=int32 | count=%u | %s\n", n, patbuf);

  size_t bytes = (size_t)n * sizeof(int);
  int *data = (int *)malloc(bytes);
  int *work = (int *)malloc(bytes);
  if (!data || !work) {
    fprintf(stderr, "Allocation failed (%zu bytes)\n", bytes);
    free(data);
    free(work);
    return 1;
  }

  // Сформируем базовый data согласно pattern
  switch (pattern) {
  case PAT_RANDOM:
    fill_random_ints(data, n, seed, VALUE_RANGE);
    break;
  case PAT_ALMOST:
    for (uint32_t i = 0; i < n; ++i)
      data[i] = (int)i; // заполним возрастающим
    make_almost_sorted(data, n, seed, param > 0 ? param : DEFAULT_ALMOST_FRAC);
    break;
  case PAT_BLOCK: {
    // Начальная последовательность — просто 0..n-1
    for (uint32_t i = 0; i < n; ++i)
      data[i] = (int)i;
    uint32_t B = (param > 0) ? (uint32_t)param : DEFAULT_BLOCK_SIZE;
    make_blockwise_shuffled(data, n, seed, B);
    break;
  }
  case PAT_STRIDE: {
    for (uint32_t i = 0; i < n; ++i)
      data[i] = (int)i;
    uint32_t K = (param > 0) ? (uint32_t)param : DEFAULT_STRIDE_K;
    make_stride_shuffled(data, n, seed, K);
    break;
  }
  case PAT_DUPS: {
    int dr = (param > 0) ? (int)param : DEFAULT_DUP_RANGE;
    make_many_duplicates(data, n, seed, dr);
    break;
  }
  default:
    fill_random_ints(data, n, seed, VALUE_RANGE);
  }

  // warmup
  for (int w = 0; w < WARMUP_TRIALS; w++) {
    memcpy(work, data, bytes);
    sort(work, 0, n, CMP_PTR, sizeof(int));
  }

  double best_ms = 1e300, sum_ms = 0.0;
  for (int t = 1; t <= trials; t++) {
    // для честности — одинаковый input в каждом прогоне:
    memcpy(work, data, bytes);

#if COUNT_COMPARISONS
    g_cmp_count = 0;
#endif

    print_sample("before", work, n);
    uint64_t t0 = now_ns();
    sort(work, 0, n, CMP_PTR, sizeof(int));
    uint64_t t1 = now_ns();
    print_sample("after ", work, n);

    if (!is_sorted_ints(work, n)) {
      fprintf(stderr, "[trial %d] NOT SORTED!\n", t);
      free(data);
      free(work);
      return 2;
    }

    double ms = (t1 - t0) / 1e6;
    sum_ms += ms;
    if (ms < best_ms)
      best_ms = ms;

    char buf[64];
    humanize(ms, buf, sizeof buf);

#if COUNT_COMPARISONS
    printf("[trial %d] elapsed: %s | comparisons=%" PRIu64 "\n", t, buf,
           g_cmp_count);
#else
    printf("[trial %d] elapsed: %s\n", t, buf);
#endif
  }

  char bbuf[64], abuf[64];
  humanize(best_ms, bbuf, sizeof bbuf);
  humanize(sum_ms / trials, abuf, sizeof abuf);
  printf("Best: %s | Avg: %s\n", bbuf, abuf);

  free(data);
  free(work);
  return 0;
}
