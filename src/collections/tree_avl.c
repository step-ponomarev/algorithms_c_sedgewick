#include "tree.h"
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

Node *_create_node(const void *val, const size_t size);
Node *_find_parent(Node *node, const void *val, Comparator *comparator);
Node *_find_node(Node *node, const void *val, Comparator *comparator);
Node *_find_min_node(Node *node);
Node *_find_max_node(Node *node);
void _rebalance_after_insert(Node *parent, Node **root);
void _rebalance_after_remove(Node *parent, bool from_left, Node **root);
void _rotate_left(Node *node, Node **root);
void _rotate_right(Node *node, Node **root);
bool _add_node(Node *sub_tree, Node *node, Comparator *comparator, Node **root);
void _check_not_null(const void *tree);
void _destroy_node(Node *node);
void _destroy_root(Node *root);
void _traversal(const Node *node, Visiter *visiter, const Order order);

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

typedef struct ValidationResult {
  bool valid;
  int height;
} ValidationResult;

static ValidationResult _validate_node(const Node *node, Comparator *comparator,
                                       const void *min_val,
                                       const void *max_val) {
  if (node == NULL) {
    return (ValidationResult){.valid = true, .height = 0};
  }

  if (min_val && comparator(node->val, min_val) <= 0) {
    return (ValidationResult){.valid = false, .height = 0};
  }
  if (max_val && comparator(node->val, max_val) >= 0) {
    return (ValidationResult){.valid = false, .height = 0};
  }

  ValidationResult left =
      _validate_node(node->left, comparator, min_val, node->val);
  if (!left.valid)
    return left;

  ValidationResult right =
      _validate_node(node->right, comparator, node->val, max_val);
  if (!right.valid)
    return right;

  int balance = right.height - left.height;
  if (balance < -1 || balance > 1) {
    return (ValidationResult){.valid = false, .height = 0};
  }

  if ((int)node->balance_factor !=
      (balance < 0 ? LEFT : (balance > 0 ? RIGHT : EQUALS))) {
    return (ValidationResult){.valid = false, .height = 0};
  }

  return (ValidationResult){
      .valid = true,
      .height = 1 + (left.height > right.height ? left.height : right.height)};
}

bool tree_validate(const Tree *tree) {
  _check_not_null(tree);
  if (tree->root == NULL)
    return tree->size == 0;

  return ((ValidationResult)_validate_node(tree->root, tree->comparator, NULL,
                                           NULL))
      .valid;
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

bool tree_remove(Tree *tree, const void *val) {
  _check_not_null(tree);

  Node *node_for_removal = _find_node(tree->root, val, tree->comparator);
  if (node_for_removal == NULL) {
    return false;
  }

  Node **ptr_to_removing_node =
      node_for_removal->parent == NULL ? &tree->root
      : node_for_removal->parent->left == node_for_removal
          ? &node_for_removal->parent->left
          : &node_for_removal->parent->right;

  if (tree->size == 1) {
    tree->root = NULL;

  } else if (node_for_removal->left == NULL &&
             node_for_removal->right == NULL) {
    bool from_left = node_for_removal->parent->left == node_for_removal;
    *ptr_to_removing_node = NULL;
    _rebalance_after_remove(node_for_removal->parent, from_left, &tree->root);
  } else if (node_for_removal->left != NULL &&
             node_for_removal->right != NULL) {

    Node *min = _find_min_node(node_for_removal->right);
    void *tmp_val = node_for_removal->val;
    node_for_removal->val = min->val;

    bool from_left = (min->parent->left == min);
    if (from_left) {
      min->parent->left = min->right;
    } else {
      min->parent->right = min->right;
    }
    if (min->right != NULL) {
      min->right->parent = min->parent;
    }

    _rebalance_after_remove(min->parent, from_left, &tree->root);
    node_for_removal = min;
  } else {
    Node *child = node_for_removal->left ? node_for_removal->left
                                         : node_for_removal->right;
    bool from_left = node_for_removal->parent &&
                     node_for_removal->parent->left == node_for_removal;

    *ptr_to_removing_node = child;
    child->parent = node_for_removal->parent;

    _rebalance_after_remove(node_for_removal->parent, from_left, &tree->root);
  }

  tree->size--;
  _destroy_node(node_for_removal);
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

void _rebalance_after_remove(Node *parent, bool from_left, Node **root) {
  Node *a = parent;

  while (a != NULL) {
    if (a->balance_factor == EQUALS) {
      a->balance_factor = from_left ? RIGHT : LEFT; // the same height
      return;
    }

    bool next_from_left = a->parent != NULL && (a->parent->left == a);
    Node *next_a = a->parent;

    if (a->balance_factor == LEFT && from_left ||
        a->balance_factor == RIGHT && !from_left) {
      a->balance_factor = EQUALS;
    } else {
      Node *b = a->balance_factor == LEFT ? a->left : a->right;

      if (b->balance_factor == EQUALS) {
        if (a->balance_factor == LEFT) {
          _rotate_right(a, root);
          a->parent->balance_factor = RIGHT;
          a->balance_factor = LEFT;
        } else {
          _rotate_left(a, root);
          a->parent->balance_factor = LEFT;
          a->balance_factor = RIGHT;
        }
        return;
      }

      Node *c = NULL;
      if (a->balance_factor == LEFT && b->balance_factor == LEFT ||
          a->balance_factor == RIGHT && b->balance_factor == RIGHT) {

        b->balance_factor == LEFT ? _rotate_right(a, root)
                                  : _rotate_left(a, root);

        a->balance_factor = b->balance_factor = EQUALS;
      } else if (a->balance_factor == LEFT && b->balance_factor == RIGHT) {
        Node *c = b->right;

        _rotate_left(b, root);
        _rotate_right(a, root);

        if (c == NULL || c->balance_factor == EQUALS) {
          a->balance_factor = EQUALS;
          b->balance_factor = EQUALS;
        } else if (c->balance_factor == LEFT) {
          a->balance_factor = RIGHT;
          b->balance_factor = EQUALS;
        } else {
          a->balance_factor = EQUALS;
          b->balance_factor = LEFT;
        }

        c != NULL && (c->balance_factor = EQUALS);
      } else {
        Node *c = b->left;

        _rotate_right(b, root);
        _rotate_left(a, root);

        if (c == NULL || c->balance_factor == EQUALS) {
          a->balance_factor = EQUALS;
          b->balance_factor = EQUALS;
        } else if (c->balance_factor == RIGHT) {
          a->balance_factor = LEFT;
          b->balance_factor = EQUALS;
        } else { // LEFT
          a->balance_factor = EQUALS;
          b->balance_factor = RIGHT;
        }
        c != NULL && (c->balance_factor = EQUALS);
      }
    }

    from_left = next_from_left;
    a = next_a;
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
