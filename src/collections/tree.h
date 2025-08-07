#ifndef TREE_H_
#define TREE_H_

#include "comparator.h"
#include <stdbool.h>
#include <stddef.h>

typedef enum Order { NLR = 0, NRL = 1, LNR = 2, RNL = 3 } Order;

typedef struct Tree Tree;

typedef void Visiter(const void *elem);

Tree *tree_create(const size_t element_size, Comparator *comparator);
void tree_destroy(Tree *tree);
bool tree_add(Tree *tree, const void *val);
bool tree_remove(Tree *tree, const void *val);
bool tree_contains(const Tree *tree, const void *val);
void *tree_min(const Tree *tree);
void *tree_max(const Tree *tree);
void tree_clear(Tree *tree);
void tree_traversal(const Tree *tree, Visiter *visiter, const Order order);
size_t tree_size(const Tree *tree);
bool tree_is_empty(const Tree *tree);
bool tree_validate(const Tree *tree);

#endif
