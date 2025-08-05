#include "comparator.h"
#include "tree.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum BalanceFactor { LEFT = -1, EQUALS = 0, RIGHT = 1 } BalanceFactor;
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
void _fix_balance(Node *parent, Node *node, Node **root);
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

void tree_remove(Tree *tree, const void *val) {
  _check_not_null(tree);

  // TODO: Тут нужно подумать как правильно удалять;
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

  _fix_balance(parant, node, root);

  return true;
}

void _fix_balance(Node *parent, Node *node, Node **root) {
  if (parent == NULL) {
    return;
  }

  Node *a = parent;
  Node *prev = node;
  while (a != NULL) {
    if (a->balance_factor == EQUALS) {
      a->balance_factor = a->right == prev ? RIGHT : LEFT;
      prev = a;
      a = a->parent;
      continue;
    }

    if (a->balance_factor == LEFT && a->right == prev ||
        a->balance_factor == RIGHT && a->left == prev) {
      a->balance_factor = EQUALS;
      return;
    }

    Node *b = a->left == prev ? a->left : a->right;
    Node *r = b->balance_factor == LEFT ? b->left : b->right;
    if (a->balance_factor == LEFT && b->balance_factor == LEFT) {
      _rotate_right(a, root);
    } else if (a->balance_factor == RIGHT && b->balance_factor == RIGHT) {
      _rotate_left(a, root);
    } else if (a->balance_factor == LEFT && b->balance_factor == RIGHT) {
      _rotate_left(b, root);
      _rotate_right(a, root);
    } else if (a->balance_factor == RIGHT && b->balance_factor == LEFT) {
      _rotate_right(b, root);
      _rotate_left(a, root);
    }

    if (r->balance_factor == EQUALS) {
      a->balance_factor = EQUALS;
      b->balance_factor = EQUALS;
    } else if (r->balance_factor == LEFT) {

    } else if (r->balance_factor == RIGHT) {
    }

    r->balance_factor = EQUALS;

    return;
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
