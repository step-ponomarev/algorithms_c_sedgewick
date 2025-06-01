#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUEUE_ERROR_CODE -819

typedef struct Node Node;

struct Node {
  void *val;
  Node *next;
};

struct Queue {
  Node *head;
  Node *tail;
  size_t entry_size;
  size_t size;
};

void queue_destroy_node(Node *);
void queue_error(const char *);

Queue *queue_create(const size_t entry_size) {
  Queue *queue = malloc(sizeof(Queue));
  if (queue == NULL) {
    return NULL;
  }

  queue->head = NULL;
  queue->size = 0;
  queue->entry_size = entry_size;

  return queue;
};

void queue_destroy(Queue *queue) {
  Node *curr = queue->head;
  while (curr != NULL) {
    Node *next = curr->next;
    queue_destroy_node(curr);
    curr = next;
  }

  free(queue);
}
void queue_add(Queue *queue, const void *entry) {
  Node *new_node = malloc(sizeof(Node));
  if (new_node == NULL) {
    queue_error("New node allocation is failed");
    return;
  }

  new_node->val = malloc(queue->entry_size);
  memcpy(new_node->val, entry, queue->entry_size);
  new_node->next = NULL;

  if (queue->head == NULL) {
    queue->head = new_node;
    queue->tail = new_node;
  } else {
    queue->tail->next = new_node;
    queue->tail = new_node;
  }

  queue->size++;
}

void queue_get(Queue *queue, void *dest) {
  if (queue_is_empty(queue)) {
    queue_error("Queue is empty");
    return;
  }

  Node *curr = queue->head;
  queue->head = curr->next;
  if (curr == queue->tail) {
    queue->tail = NULL;
  }

  memcpy(dest, curr->val, queue->entry_size);
  queue_destroy_node(curr);

  queue->size--;
}

char queue_is_empty(const Queue *queue) { return queue->size == 0; }

size_t queue_size(const Queue *queue) { return queue->size; }

void queue_destroy_node(Node *node) {
  if (node == NULL) {
    queue_error("Node free error, node is null");
    return;
  }

  free(node->val);
  free(node);
}

void queue_error(const char *msg) {
  fprintf(stderr, "QueueERROR: %s\n", msg);
  exit(QUEUE_ERROR_CODE);
}
