#ifndef DEQUE_H
#define DEQUE_H

#include <stddef.h>

typedef struct Deque Deque;

Deque *dequeue_create(const size_t);
void dequeue_destroy(Deque *);
void dequeue_add_first(Deque *, const void *);
void dequeue_add_last(Deque *, const void *);
void dequeue_poll_first(Deque *, void *);
void dequeue_poll_last(Deque *, void *);
char dequeue_is_empty(const Deque *);
size_t dequeue_size(const Deque *);

#endif
