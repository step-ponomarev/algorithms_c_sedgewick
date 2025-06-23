#include "deque.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// TODO: Tests
typedef struct Node Node;

struct Node {
  void *val;
  Node *prev;
  Node *next;
};

struct Deque {
  Node *head;
  Node *tail;
  size_t element_size;
  size_t size;
};

Node *_create_node(const void *, const size_t);
void _destroy_node(Node *);

Deque *dequeue_create(const size_t element_size) {
  Deque *deque = malloc(sizeof(Deque));
  deque->head = NULL;
  deque->tail = NULL;
  deque->element_size = element_size;
  deque->size = 0;

  return deque;
}
void dequeue_destroy(Deque *deque) {
  Node *node = deque->head;
  while (node != NULL) {
    Node *tmp = node;
    node = node->next;
    _destroy_node(tmp);
  }

  free(deque);
}

void dequeue_add_first(Deque *deque, const void *elem) {
  Node *node = _create_node(elem, deque->element_size);
  if (deque->size == 0) {
    deque->head = node;
    deque->tail = node;
    deque->size = 1;
    return;
  }

  node->next = deque->head;
  deque->head->prev = node;

  deque->head = node;
  deque->size++;
}

void dequeue_add_last(Deque *deque, const void *elem) {
  Node *node = _create_node(elem, deque->element_size);
  if (deque->size == 0) {
    deque->head = node;
    deque->tail = node;
    deque->size = 1;
    return;
  }

  node->prev = deque->tail;
  node->prev->next = node;

  deque->tail = node;
  deque->size++;
}

void dequeue_poll_first(Deque *deque, void *dest) {
  if (deque->size == 0) {
    return;
  }

  memcpy(dest, deque->head->val, deque->element_size);
  if (deque->size == 1) {
    _destroy_node(deque->head);
    deque->head = NULL;
    deque->tail = NULL;
    deque->size--;
    return;
  }

  Node *tmp = deque->head;
  deque->head = deque->head->next;
  deque->head->prev = NULL;
  _destroy_node(tmp);

  deque->size--;
}

void dequeue_poll_last(Deque *deque, void *dest) {
  if (deque->size == 0) {
    return;
  }

  memcpy(dest, deque->tail->val, deque->element_size);
  if (deque->size == 1) {
    _destroy_node(deque->head);
    deque->head = NULL;
    deque->tail = NULL;
    deque->size--;
    return;
  }

  Node *tmp = deque->tail;
  deque->tail = deque->tail->prev;
  deque->tail->next = NULL;

  _destroy_node(tmp);

  deque->size--;
}

inline char dequeue_is_empty(const Deque *deque) { return deque->size == 0; }

inline size_t dequeue_size(const Deque *deque) { return deque->size; }

Node *_create_node(const void *elem, const size_t elem_size) {
  Node *node = malloc(sizeof(Node));
  node->val = malloc(elem_size);
  node->prev = NULL;
  node->next = NULL;
  memcpy(node->val, elem, elem_size);

  return node;
}

void _destroy_node(Node *node) {
  free(node->val);
  free(node);
}
