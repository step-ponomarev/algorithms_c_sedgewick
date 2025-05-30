#include "stack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define START_CAPACITY 0xff
#define EXPAND_FACTOR 2

struct Stack {
  void *data;
  size_t entry_size;
  size_t size;
  size_t capacity;
};

void expand_stack(Stack *);
void shrink_stack(Stack *);

Stack *stack_create(const size_t entry_size) {
  if (entry_size == 0) {
    return NULL;
  }

  void *data = calloc(START_CAPACITY, entry_size);
  if (data == NULL) {
    return NULL;
  }

  Stack *stack = malloc(sizeof(Stack));
  if (stack == NULL) {
    free(data);
    return NULL;
  }

  stack->data = data;
  stack->entry_size = entry_size;
  stack->size = 0;
  stack->capacity = START_CAPACITY;

  return stack;
}
void stack_destroy(Stack *stack) {
  free(stack->data);
  free(stack);
}

void stack_push(Stack *stack, const void *entry) {
  if (stack->size == stack->capacity) {
    expand_stack(stack);
  }

  memcpy(stack->data + (stack->entry_size * stack->size++), entry,
         stack->entry_size);
}

void stack_pop(Stack *stack, void *dest) {
  if (stack->size == 0) {
    return;
  }

  if (stack->capacity > START_CAPACITY &&
      (stack->size < stack->capacity / EXPAND_FACTOR)) {
    shrink_stack(stack);
  }

  memcpy(dest, stack->data + (stack->entry_size * (--stack->size)),
         stack->entry_size);
}

size_t stack_size(const Stack *stack) { return stack->size; }

char stack_is_empty(const Stack *stack) { return stack->size == 0; }

void expand_stack(Stack *stack) {
  stack->capacity *= EXPAND_FACTOR;
  stack->data = realloc(stack->data, stack->capacity);
}

void shrink_stack(Stack *stack) {
  stack->capacity /= EXPAND_FACTOR;
  stack->data = realloc(stack->data, stack->capacity);
}
