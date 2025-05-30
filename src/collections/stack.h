#ifndef STACK_H
#define STACK_H

#include <stddef.h>
typedef struct Stack Stack;

Stack *stack_create(const size_t);
void stack_destroy(Stack *);
void stack_push(Stack *, const void *);
void stack_pop(Stack *, void *);
size_t stack_size(const Stack *);
size_t stack_is_empty(const Stack *);

#endif
