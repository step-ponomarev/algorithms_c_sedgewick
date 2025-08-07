#include "comparator.h"
#include "tree.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum BalanceFactor { LEFT = -1, EQUALS = 0, RIGHT = 1 } BalanceFactor;
typedef enum Direction { LEFT_DIR = 1, RIGHT_DIR = 2 } Direction;

typedef struct Node Node;

struct Node {
  void *val;
  Node *parent;
  Node *left;
  Node *right;
  BalanceFactor balance_factor;
};

typedef struct Tree {
  Node *root;
  size_t size;
  size_t element_size;
  Comparator *comparator;
} Tree;

// TODO: Отчинить повороты эти все. сделать как можно чище.

Node *_create_node(const void *val, const size_t size);
Node *_find_parent(Node *node, const void *val, Comparator *comparator);
Node *_find_node(Node *node, const void *val, Comparator *comparator);
Node *_find_min_node(Node *node);
Node *_find_max_node(Node *node);
void _rebalance_after_insert(Node *parent, Node **root);
void _rebalance_after_remove(Node *parent, Node *linked, Node **root);
void _rotate_left(Node *node, Node **root);
void _rotate_right(Node *node, Node **root);
bool _add_node(Node *sub_tree, Node *node, Comparator *comparator, Node **root);
void _check_not_null(const void *tree);
void _destroy_node(Node *node);
void _destroy_root(Node *root);
void _traversal(const Node *node, Visiter *visiter, const Order order);

// TODO: adding function with balance_factor

Tree *tree_create(const size_t element_size, Comparator *comparator) {
  _check_not_null(comparator);

  Tree *tree = malloc(sizeof(Tree));
  tree->root = NULL;
  tree->size = 0;
  tree->element_size = element_size;
  tree->comparator = comparator;

  return tree;
}

void tree_destroy(Tree *tree) {
  if (tree == NULL) {
    return;
  }

  if (tree->root != NULL) {
    _destroy_root(tree->root);
  }

  free(tree);
}

bool tree_add(Tree *tree, const void *val) {
  _check_not_null(tree);
  if (tree->size == 0) {
    tree->root = _create_node(val, tree->element_size);
    tree->size++;
    return true;
  }

  Node *added_node = _create_node(val, tree->element_size);
  if (_add_node(tree->root, added_node, tree->comparator, &tree->root)) {
    tree->size++;
    return true;
  } else {
    _destroy_node(added_node);
    return false;
  }
}

static int _get_deep(const Node *node) {
  if (node == NULL) {
    return 0;
  }

  int left = _get_deep(node->left);
  int right = _get_deep(node->right);

  return 1 + (left > right ? left : right);
}

/*
 * Вызов для всего дерева.
 * Печатает глубину и возвращает true, если дерево:
 *   - не NULL
 *   - корректно как BST
 *   - сбалансировано (AVL)
 *   - имеет консистентные balance_factor
 */
bool tree_validate(const Tree *tree) {
  _check_not_null(tree);
  if (tree->size == 0) {
    return true;
  }

  // int left = _get_deep(tree->root->left);
  // int right = _get_deep(tree->root->right);

  printf("L %d | R %d\n", left, right);

  // int diff = abs(left - right);

  return true;
}

bool tree_remove(Tree *tree, const void *val) {
  _check_not_null(tree);

  Node *node = _find_node(tree->root, val, tree->comparator);
  if (node == NULL) {
    return false;
  }

  if (tree->size == 1) {
    _destroy_node(tree->root);
    tree->root = NULL;
    tree->size = 0;
    return true;
  }

  if (node == tree->root && tree->root->left == NULL ||
      tree->root->right == NULL) {
    tree->root =
        tree->root->left == NULL ? tree->root->right : tree->root->left;
    tree->root->parent = NULL;
    tree->size--;

    _destroy_node(node);
    return true;
  }

  const Direction dir = node->parent->left == node ? LEFT_DIR : RIGHT_DIR;
  Node **link_to_current_node;
  if (node == tree->root) {
    link_to_current_node = &tree->root;
  } else {
    link_to_current_node =
        dir == LEFT_DIR ? &node->parent->left : &node->parent->right;
  }

  Node *parent = node->parent;
  if (node->left == NULL && node->right == NULL) {
    *link_to_current_node = NULL;
    _rebalance_after_remove(parent, NULL, &tree->root);
  } else if (node->left == NULL || node->right == NULL) {
    Node *next = node->left == NULL ? node->right : node->left;
    next->parent = parent;
    *link_to_current_node = next;
    _rebalance_after_remove(parent, next, &tree->root);
  } else {
    Node *to_add = _find_parent(node->right, node->left->val, tree->comparator);
    *link_to_current_node = node->right;
    node->right->parent = parent;

    to_add->left = node->left;
    node->left->parent = to_add;
    _rebalance_after_remove(to_add, to_add->left, &tree->root);
  }

  _destroy_node(node);
  tree->size--;

  return true;
}

bool tree_contains(const Tree *tree, const void *val) {
  _check_not_null(tree);
  return _find_node(tree->root, val, tree->comparator) == NULL ? false : true;
}
void *tree_min(const Tree *tree) {
  _check_not_null(tree);

  Node *min_node = _find_min_node(tree->root);
  if (min_node == NULL) {
    return NULL;
  }

  void *res = malloc(tree->element_size);
  memcpy(res, min_node->val, tree->element_size);
  return res;
}

void *tree_max(const Tree *tree) {
  _check_not_null(tree);

  Node *max_node = _find_max_node(tree->root);
  if (max_node == NULL) {
    return NULL;
  }

  void *res = malloc(tree->element_size);
  memcpy(res, max_node->val, tree->element_size);
  return res;
}

void tree_clear(Tree *tree) {
  _check_not_null(tree);

  _destroy_root(tree->root);
  tree->root = NULL;
  tree->size = 0;
}

void tree_traversal(const Tree *tree, Visiter *visiter, const Order order) {
  _check_not_null(tree);
  _check_not_null(visiter);

  if (tree->root == NULL) {
    return;
  }

  _traversal(tree->root, visiter, order);
}

void _traversal(const Node *node, Visiter *visiter, const Order order) {
  if (node == NULL) {
    return;
  }

  if (order == LNR) {
    _traversal(node->left, visiter, order);
    visiter(node->val);
    _traversal(node->right, visiter, order);
    return;
  }

  if (order == NLR) {
    visiter(node->val);
    _traversal(node->left, visiter, order);
    _traversal(node->right, visiter, order);
    return;
  }

  if (order == NRL) {
    visiter(node->val);
    _traversal(node->right, visiter, order);
    _traversal(node->left, visiter, order);
    return;
  }

  if (order == RNL) {
    _traversal(node->right, visiter, order);
    visiter(node->val);
    _traversal(node->left, visiter, order);
    return;
  }

  fprintf(stderr, "Unsupported order: %d\n", order);
  exit(1);
}

size_t tree_size(const Tree *tree) {
  _check_not_null(tree);
  return tree->size;
}

bool tree_is_empty(const Tree *tree) {
  _check_not_null(tree);
  return tree->size == 0;
}

Node *_create_node(const void *val, const size_t size) {
  Node *node = malloc(sizeof(Node));
  node->val = malloc(size);
  memcpy(node->val, val, size);

  node->parent = NULL;
  node->left = NULL;
  node->right = NULL;
  node->balance_factor = 0;

  return node;
}

Node *_find_node(Node *node, const void *val, Comparator *comparator) {
  if (node == NULL) {
    return NULL;
  }

  const int res = comparator(node->val, val);
  if (res == 0) {
    return node;
  }

  return res > 0 ? _find_node(node->left, val, comparator)
                 : _find_node(node->right, val, comparator);
}

Node *_find_parent(Node *node, const void *val, Comparator *comparator) {
  const int res = comparator(node->val, val);

  if (res == 0) {
    return node->parent;
  }

  if (res < 0) {
    return node->right == NULL ? node
                               : _find_parent(node->right, val, comparator);
  }

  return node->left == NULL ? node : _find_parent(node->left, val, comparator);
}

Node *_find_min_node(Node *node) {
  if (node == NULL) {
    return NULL;
  }

  if (node->left == NULL) {
    return node;
  }

  return _find_min_node(node->left);
}

Node *_find_max_node(Node *node) {
  if (node == NULL) {
    return NULL;
  }

  if (node->right == NULL) {
    return node;
  }

  return _find_max_node(node->right);
}

bool _add_node(Node *sub_tree, Node *node, Comparator *comparator,
               Node **root) {
  Node *parant = _find_parent(sub_tree, node->val, comparator);
  if (parant == NULL) {
    return false;
  }

  const int res = comparator(parant->val, node->val);
  Node **added_node = res < 0 ? &(parant->right) : &(parant->left);
  if (*added_node != NULL) {
    return false;
  }
  node->parent = parant;
  *added_node = node;

  _rebalance_after_insert(parant, root);

  return true;
}

void _rebalance_after_insert(Node *node, Node **root) {
  if (node == NULL) {
    return;
  }

  if (node->left != NULL && node->right != NULL ||
      node->left == NULL && node->right == NULL) {
    node->balance_factor = EQUALS;
    return;
  }

  Node *a = node;
  Node *b = node->left == NULL ? node->right : node->left;
  while (a != NULL) {
    if (a->balance_factor == EQUALS) {
      a->balance_factor = a->right == b ? RIGHT : LEFT;
      b = a;
      a = a->parent;
      continue;
    }

    if (a->balance_factor == LEFT && a->right == b ||
        a->balance_factor == RIGHT && a->left == b) {
      a->balance_factor = EQUALS;
      return;
    }

    if (a->balance_factor == LEFT && b->balance_factor == LEFT ||
        a->balance_factor == RIGHT && b->balance_factor == RIGHT) {

      b->balance_factor == LEFT ? _rotate_right(a, root)
                                : _rotate_left(a, root);

    } else if (a->balance_factor == LEFT && b->balance_factor == RIGHT) {
      _rotate_left(b, root);
      _rotate_right(a, root);
    } else {
      _rotate_right(b, root);
      _rotate_left(a, root);
    }

    a->balance_factor = b->balance_factor = EQUALS;
    return;
  }
}

void _rebalance_after_remove(Node *parent, Node *linked, Node **root) {
  if (parent == NULL) {
    return;
  }

  // rmoeved list, and has another - deep is the same
  if (parent->balance_factor == EQUALS) {
    parent->balance_factor = parent->left == NULL ? RIGHT : LEFT;
    return;
  }

  Node *a = parent;
  Node *b = NULL;
  if (a->left != NULL && a->right != NULL) {
    b = a->left->left != NULL || a->left->right != NULL ? a->left : a->right;
  } else if (a->left != NULL || a->right != NULL) {
    b = a->left == NULL ? a->right : a->left;
  }

  while (a != NULL) {
    if (a->left == NULL && a->right == NULL) {
      a->balance_factor = EQUALS;
      if (b != NULL && b->balance_factor == EQUALS) {
        return;
      }

      b = a;
      a = a->parent;
      continue;
    }

    if (a->balance_factor == EQUALS) {
      a->balance_factor = a->left == b ? RIGHT : LEFT;
    }

    if (b->balance_factor == EQUALS) {
      a->balance_factor += a->left == b ? RIGHT : LEFT;
      b = a;
      a = a->parent;
      continue;
    }

    if (a->balance_factor == RIGHT && b->balance_factor == RIGHT ||
        a->balance_factor == LEFT && b->balance_factor == LEFT) {
      Node *a_parent = a->parent;

      a->balance_factor == RIGHT ? _rotate_left(a, root)
                                 : _rotate_right(a, root);
      a->balance_factor = EQUALS;
      b->balance_factor = EQUALS;
      a = a_parent;
      continue;
    }

    Node *r = b->balance_factor == EQUALS ? NULL
              : b->balance_factor == LEFT ? b->left
                                          : b->right;
    // a right, b left OR a left, b right
    Node *prev_parent = a->parent;
    const bool from_left = prev_parent != NULL && prev_parent->left == a;
    const bool right = a->balance_factor == RIGHT;
    if (right) {
      _rotate_right(b, root);
      _rotate_left(a, root);
    } else {
      _rotate_left(b, root);
      _rotate_right(a, root);
    }

    if (r == NULL || r->balance_factor == EQUALS) {
      a->balance_factor = b->balance_factor = EQUALS;
    } else if (r->balance_factor == LEFT) {
      a->balance_factor = b->balance_factor = LEFT;
    } else {
      a->balance_factor = b->balance_factor = RIGHT;
    }
    r->balance_factor = EQUALS;

    a = prev_parent;
    b = a == NULL ? NULL : from_left ? a->left : a->right;
  }
}

void _rotate_left(Node *node, Node **root) {
  Node *new_sub_root = node->right;
  Node *prev_left = new_sub_root->left;
  new_sub_root->left = node;
  node->right = prev_left;

  if (prev_left != NULL) {
    prev_left->parent = node;
  }

  new_sub_root->parent = node->parent;
  if (node->parent == NULL) {
    *root = new_sub_root;
  } else {
    if (node->parent->left == node) {
      node->parent->left = new_sub_root;
    } else {
      node->parent->right = new_sub_root;
    }
  }

  node->parent = new_sub_root;
}

void _rotate_right(Node *node, Node **root) {
  Node *new_sub_root = node->left;
  Node *prev_right = new_sub_root->right;
  new_sub_root->right = node;
  node->left = prev_right;

  if (prev_right != NULL) {
    prev_right->parent = node;
  }

  new_sub_root->parent = node->parent;
  if (node->parent == NULL) {
    *root = new_sub_root;
  } else {
    if (node->parent->left == node) {
      node->parent->left = new_sub_root;
    } else {
      node->parent->right = new_sub_root;
    }
  }
  node->parent = new_sub_root;
}

void _destroy_root(Node *root) {
  if (root == NULL) {
    return;
  }

  _destroy_root(root->left);
  _destroy_root(root->right);
  _destroy_node(root);
}

void _destroy_node(Node *node) {
  if (node == NULL) {
    return;
  }

  if (node->val != NULL) {
    free(node->val);
    node->val = NULL;
  }

  free(node);
}

void _check_not_null(const void *ptr) {
  if (ptr != NULL) {
    return;
  }

  fprintf(stderr, "NullPointerError\n");
  exit(1);
}
