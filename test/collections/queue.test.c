#include "../../src/collections/queue.h"
#include <stddef.h>
#include <stdio.h>

int main() {
  const size_t max_size = 200;
  Queue *queue = queue_create(sizeof(int));

  for (int i = 0; i < max_size; i++) {
    queue_add(queue, &i);
  }

  int fromqueue;
  for (int i = 0; i < max_size; i++) {
    queue_get(queue, &fromqueue);
    printf("%d ", fromqueue);
  }
  printf("\n");

  printf("Queue is empty: %d", queue_is_empty(queue));

  queue_destroy(queue);

  return 0;
}
