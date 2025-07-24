#include "list.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node Node;

struct Node {
  void *val;
  Node *next;
};

struct List {
  size_t element_size;
  size_t size;
  Node *head;
  Node *tail;
};

void destroy_node(Node *node);
Node *create_node(const size_t element_size, const void *elem);

List *list_create(const size_t element_size) {
  List *list = malloc(sizeof(List));
  list->element_size = element_size;
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;

  return list;
}

void list_destroy(List *list) {
  Node *curr = list->head;

  while (curr != NULL) {
    Node *next = curr->next;
    destroy_node(curr);

    curr = next;
  }

  free(list);
}

void list_add(List *list, const void *elem) {
  if (list->head == NULL) {
    list->head = list->tail = create_node(list->element_size, elem);
    list->size = 1;
    return;
  }

  list->tail->next = create_node(list->element_size, elem);
  list->tail = list->tail->next;
  list->size++;
}

void list_get_item(const List *list, const size_t index, void *dest) {
  if (list == NULL || dest == NULL || list->size <= index) {
    return;
  }

  Node *curr = list->head;
  for (size_t i = 0; i < index; i++) {
    curr = curr->next;
  }

  memcpy(dest, curr->val, list->element_size);
}

void list_remove_item(List *list, const size_t index, void *dest) {
  if (dest == NULL || list->size <= index) {
    return;
  }

  Node *prev = list->head;
  Node *curr = list->head;
  for (size_t i = 0; i < index; i++) {
    prev = curr;
    curr = curr->next;
  }

  prev->next = curr->next;
  if (curr == list->head) {
    list->head = prev->next;
  }

  if (curr == list->tail) {
    list->tail = prev->next;
  }
  list->size--;

  memcpy(dest, curr->val, list->element_size);
}

size_t list_size(const List *list) { return list->size; }

bool list_is_empty(const List *list) { return list->size == 0; }

void destroy_node(Node *node) {
  if (node == NULL) {
    return;
  }

  free(node->val);
  free(node);
}

Node *create_node(const size_t element_size, const void *elem) {
  Node *node = malloc(sizeof(Node));
  node->next = NULL;

  node->val = malloc(element_size);
  memcpy(node->val, elem, element_size);

  return node;
}
