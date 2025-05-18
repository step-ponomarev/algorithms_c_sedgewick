#include "../../lib/point.h"
#include <stdio.h>
#include <stdlib.h>

#define ITEM_T point
#include "../../lib/list.h"

#define max(a, b) (a > b ? a : b)

extern float rand_float();
extern void **malloc2d(const size_t row, const size_t col,
                       const size_t cell_size);
extern void free2d(void **, const size_t);

static int G;
static int GRID_SIZE;
static float d;
static link **GRID;

static int *ID;
static int *SZ;

static int largest_network_count = 0;

void insert_point(const point);
int find_root(int i);

int main(const int argc, const char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Expected int 'N' and float 'd' args\n");
    return -1;
  }

  const int N = atoi(argv[1]);
  d = atof(argv[2]);
  if (d <= 0 || d >= 1) {
    fprintf(stderr, "Distance must be in range (0, 1)\n");
    return -1;
  }

  G = 1 / d;
  GRID_SIZE = G + 2;

  GRID = (link **)malloc2d(GRID_SIZE, GRID_SIZE, sizeof(link));
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      GRID[i][j] = NULL;
    }
  }

  ID = malloc(sizeof(int) * GRID_SIZE * GRID_SIZE);
  SZ = malloc(sizeof(int) * GRID_SIZE * GRID_SIZE);
  for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
    ID[i] = i;
    SZ[i] = 0;
  }

  for (int i = 0; i < N; i++) {
    insert_point((point){.x = rand_float(), .y = rand_float()});
  }

  printf("Network has %d nodes connected by ribs len < %f\n",
         largest_network_count, d);

  free(ID);
  free(SZ);
  free2d((void **)GRID, GRID_SIZE);

  return 0;
}

void insert_point(const point p) {
  const int X = (int)(p.x * G) + 1;
  const int Y = (int)(p.y * G) + 1;

  const int pr = find_root(X * (GRID_SIZE - 1) + Y);
  SZ[pr]++;
  largest_network_count = max(SZ[pr], largest_network_count);

  for (int i = X - 1; i <= X + 1; i++) {
    for (int j = Y - 1; j <= Y + 1; j++) {
      for (link s = GRID[i][j]; s != NULL; s = next(s)) {
        const point p2 = get_item(s);
        if (distance(&p, &p2) >= d) {
          continue;
        }

        const int qr = find_root(i * (GRID_SIZE - 1) + j);
        if (qr == pr) {
          continue;
        }

        if (SZ[pr] > SZ[qr]) {
          ID[qr] = pr;
          SZ[pr] += SZ[qr];
          largest_network_count = max(SZ[pr], largest_network_count);
        } else {
          ID[pr] = qr;
          largest_network_count = max(SZ[qr], largest_network_count);
        }
      }
    }
  }

  if (GRID[X][Y] == NULL) {
    GRID[X][Y] = create(p);
  } else {
    insert_next(GRID[X][Y], p);
  }
}

int find_root(int i) {
  while (ID[i] != i) {
    ID[i] = ID[ID[i]];
    i = ID[i];
  }

  return i;
}
