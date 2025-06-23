#include "deque.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define INIT_CAPACITY 16
#define EXPAND_FACTOR 2

struct Deque {
  void *data;
  size_t element_size;
  size_t head_i;
  size_t tail_i;
  size_t size;
  size_t capacity;
};

bool _need_expand(const Deque *);
void _expand(Deque *);
void _normalize(Deque *);

Deque *dequeue_create(const size_t element_size) {
  Deque *deque = malloc(sizeof(Deque));
  deque->element_size = element_size;
  deque->capacity = INIT_CAPACITY;
  deque->data = calloc(deque->capacity, deque->element_size);
  deque->size = 0;
  deque->head_i = deque->tail_i = (deque->capacity / 2) - 1; // on center

  return deque;
}
void dequeue_destroy(Deque *deque) {
  free(deque->data);
  free(deque);
}

void dequeue_add_first(Deque *deque, const void *elem) {
  if (_need_expand(deque)) {
    _expand(deque);
  }

  if (deque->head_i == 0) {
    _normalize(deque);
  }

  if (deque->size == 0) {
    memcpy((uint8_t *)deque->data + deque->head_i * deque->element_size, elem,
           deque->element_size);
    deque->size++;
    return;
  }

  memcpy((uint8_t *)deque->data + (--deque->head_i) * deque->element_size, elem,
         deque->element_size);
  deque->size++;
}

void dequeue_add_last(Deque *deque, const void *elem) {
  if (_need_expand(deque)) {
    _expand(deque);
  }

  if (deque->tail_i == deque->capacity - 1) {
    _normalize(deque);
  }

  if (deque->size == 0) {
    memcpy((uint8_t *)deque->data + deque->head_i * deque->element_size, elem,
           deque->element_size);
    deque->size++;
    return;
  }

  memcpy((uint8_t *)deque->data + (++deque->tail_i) * deque->element_size, elem,
         deque->element_size);
  deque->size++;
}

void dequeue_poll_first(Deque *deque, void *dest) {
  if (deque->size == 0) {
    return;
  }

  memcpy(dest, (uint8_t *)deque->data + (deque->head_i++) * deque->element_size,
         deque->element_size);
  deque->size--;
}

void dequeue_poll_last(Deque *deque, void *dest) {
  if (deque->size == 0) {
    return;
  }

  memcpy(dest, (uint8_t *)deque->data + (deque->tail_i--) * deque->element_size,
         deque->element_size);
  deque->size--;
}

inline char dequeue_is_empty(const Deque *deque) {
  return dequeue_size(deque) == 0;
}

inline size_t dequeue_size(const Deque *deque) { return deque->size; }

inline bool _need_expand(const Deque *deque) {
  return deque->size >= deque->capacity - 2;
}

void _expand(Deque *deque) {
  deque->capacity *= EXPAND_FACTOR;
  deque->data = realloc(deque->data, deque->capacity);
  _normalize(deque);
}

void _normalize(Deque *deque) {
  size_t new_head = (deque->capacity - deque->size) << 1;
  memcpy((uint8_t *)deque->data + new_head * deque->element_size,
         deque->data + deque->head_i * deque->element_size,
         deque->size * deque->element_size); // move to center
  deque->head_i = new_head;
  deque->tail_i = deque->tail_i + new_head;
}
