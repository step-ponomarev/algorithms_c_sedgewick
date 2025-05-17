#include "list.h"
#include <stdlib.h>

struct node {
  item item;
  link next;
  link prev;
};

link create(const item item) {
  link link = malloc(sizeof(node));
  link->item = item;
  link->next = link->prev = NULL;

  return link;
}

void free_node(link l) {
  if (l == NULL) {
    return;
  }

  free(l);
}

void insert_next(link l, const item item) {
  if (l == NULL) {
    return;
  }

  link nl = create(item);
  nl->next = l->next;
  if (nl->next) {
    nl->next->prev = nl;
  }

  nl->prev = l;
  l->next = nl;
}

link delete_curr(link l) {
  if (l == NULL) {
    return NULL;
  }

  if (l->prev) {
    l->prev->next = l->next;
  }

  if (l->next) {
    l->next->prev = l->prev;
  }

  return l;
}

link delete_next(link l) {
  if (l == NULL) {
    return NULL;
  }

  return delete_curr(l->next);
}

void swap(link l1, link l2) {
  if (l1 == NULL || l2 == NULL || l1 == l2) {
    return;
  }

  insert_next(l1, get_item(l2));
  delete_curr(l1);

  insert_next(l2, get_item(l1));
  delete_curr(l2);
}

link next(const link l) {
  if (l == NULL) {
    return NULL;
  }

  return l->next;
}

inline item get_item(const link l) { return l->item; }
