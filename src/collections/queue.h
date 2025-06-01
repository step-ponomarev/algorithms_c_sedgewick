#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

typedef struct Queue Queue;

Queue *queue_create(const size_t);
void queue_destroy(Queue *);
void queue_add(Queue *, const void *);
void queue_get(Queue *, void *);
char queue_is_empty(const Queue *);
size_t queue_size(const Queue *);

#endif
