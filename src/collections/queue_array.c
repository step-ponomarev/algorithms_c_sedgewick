#include "queue.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>

#define QUEUE_ERROR_CODE -812
#define INIT_CAPACITY 1
#define EXPAND_FACTOR 2

#define queue_get_tail_index(q) (q->head_index + q->size - 1)

// fixed array implementation first
struct Queue {
  void *array;
  size_t size;
  size_t entry_size;
  size_t head_index;
  size_t capacity;
};

void queue_null_check(const Queue *);
void queue_error(const char *);
char queue_is_need_expand(const Queue *);
void queue_expand(Queue *);

char queue_is_need_shring(const Queue *);
void queue_shrink(Queue *);

Queue *queue_create(const size_t entry_size) {
  if (entry_size == 0) {
    queue_error("Entry size must be > 0");
    return NULL;
  }

  Queue *queue = malloc(sizeof(Queue));
  if (queue == NULL) {
    return NULL;
  }

  queue->capacity = INIT_CAPACITY;
  queue->array = calloc(queue->capacity, entry_size);
  queue->head_index = queue->size = 0;
  queue->entry_size = entry_size;

  return queue;
}

void queue_destroy(Queue *queue) {
  queue_null_check(queue);

  free(queue->array);
  free(queue);
}

void queue_add(Queue *queue, const void *entry) {
  queue_null_check(queue);

  if (queue_is_need_expand(queue)) {
    queue_expand(queue);
  }

  void *tail =
      queue->array + queue->entry_size * (queue_get_tail_index(queue) + 1);
  memcpy(tail, entry, queue->entry_size);
  queue->size++;
}

void queue_get(Queue *queue, void *dest) {
  queue_null_check(queue);

  if (queue_is_need_shring(queue)) {
    queue_shrink(queue);
  }

  void *head = queue->array + queue->head_index * queue->entry_size;
  memcpy(dest, head, queue->entry_size);
  queue->head_index++;
  queue->size--;
};

char queue_is_empty(const Queue *queue) {
  queue_null_check(queue);

  return queue->size == 0;
}

size_t queue_size(const Queue *queue) {
  queue_null_check(queue);

  return queue->size;
}

char queue_is_need_expand(const Queue *queue) {
  return queue_get_tail_index(queue) == queue->capacity - 1;
}

void queue_expand(Queue *queue) {
  queue->capacity *= EXPAND_FACTOR;
  queue->array = realloc(queue->array, queue->capacity * queue->entry_size);
}

char queue_is_need_shring(const Queue *queue) {
  return queue->capacity > INIT_CAPACITY && queue->head_index != 0 &&
         (queue->size < queue->capacity / EXPAND_FACTOR);
}

void queue_shrink(Queue *queue) {
  queue->capacity /= EXPAND_FACTOR;
  void *new_arr = calloc(queue->capacity, queue->entry_size);

  const void *head = queue->array + queue->head_index * queue->entry_size;
  const size_t size_to_copy =
      (queue_get_tail_index(queue) - queue->head_index + 1) * queue->entry_size;
  memcpy(new_arr, head, size_to_copy);

  free(queue->array);
  queue->array = new_arr;
  queue->head_index = 0;
}

void queue_null_check(const Queue *queue) {
  if (queue != NULL) {
    return;
  }

  queue_error("Queue is null");
}

void queue_error(const char *msg) {
  fprintf(stderr, "QueueERROR: %s\n", msg);
  exit(QUEUE_ERROR_CODE);
};
