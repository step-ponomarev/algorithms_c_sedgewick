
#include "../lib/rand_utils.h"
#include "list.h"
#include "queue.h"
#include <stddef.h>
#include <stdlib.h>

struct Queue {
  List *list;
};

Queue *queue_create(const size_t element_size) {
  Queue *queue = malloc(sizeof(Queue));
  queue->list = list_create(element_size);

  return queue;
}

void queue_destroy(Queue *queue) {
  list_destroy(queue->list);
  free(queue);
}

void queue_add(Queue *queue, const void *elem) { list_add(queue->list, elem); }

void queue_get(Queue *queue, void *dest) {
  list_remove_item(queue->list, random_int(0, list_size(queue->list)), dest);
}

char queue_is_empty(const Queue *queue) { return list_is_empty(queue->list); }
size_t queue_size(const Queue *queue) { return list_size(queue->list); }
