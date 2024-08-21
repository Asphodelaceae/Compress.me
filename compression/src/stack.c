#include "stack.h"
#include "node.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Stack {
  uint32_t top;
  uint32_t capacity;
  Node **items;
} Stack;

Stack *stack_create(uint32_t capacity) {
  Stack *s = (Stack *)malloc(sizeof(Stack));
  if (s) {
    s->items = (Node **)calloc(capacity, sizeof(Node *));
    if (s->items) {
      s->capacity = capacity;
      s->top = 0;
    } else {
      free(s->items);
      s->items = NULL;
      free(s);
      s = NULL;
      return (s);
    }
  } else {
    free(s);
    s = NULL;
  }
  return (s);
}

void stack_delete(Stack **s) {
  if (*s) {
    if ((*s)->items) {
      free((*s)->items);
      (*s)->items = NULL;
    }
    free(*s);
    (*s) = NULL;
  }
}

bool stack_empty(Stack *s) {
  if (s) {
    if (s->top == 0) {
      return true;
    }
  }
  return false;
}

bool stack_full(Stack *s) {
  if (s) {
    if (s->top == s->capacity) {
      return true;
    }
  }
  return false;
}

uint32_t stack_size(Stack *s) {
  if (s) {
    return (s->top);
  }
  return 0;
}

bool stack_push(Stack *s, Node *n) {
  if (s && n) {
    if (stack_full(s)) {
      return false;
    }
    s->items[s->top] = n;
    s->top += 1;
    return true;
  }
  return false;
}

bool stack_pop(Stack *s, Node **n) {
  if (s) {
    if (stack_empty(s)) {
      return false;
    }
    (*n) = s->items[s->top - 1];
    s->items[s->top - 1] = 0;
    s->top -= 1;
    return true;
  }
  (*n) = NULL;
  return false;
}

void stack_print(Stack *s) {
  if (s) {
    for (uint32_t i = 0; i < stack_size(s); i += 1) {
      node_print(s->items[i]);
    }
  }
}

/*
int main(void){
        Stack *s = stack_create(20);
        Node *a = node_create('a', 25);
        Node *b = node_create('b', 7);
        Node *c = node_create('c', 28);
        Node *d = node_create('d', 32);
        Node *e = node_create('e', 5);
        Node *o1 = NULL;
        Node *o2 = NULL;
        Node *o3 = NULL;
        Node *o4 = NULL;
        Node *o5 = NULL;
        stack_push(s, a);
        stack_push(s, c);
        stack_push(s, b);
        stack_push(s, e);
        stack_push(s, d);
        stack_print(s);
        printf("sep\n");
        stack_pop(s, &o1);
        stack_pop(s, &o2);
        stack_pop(s, &o3);
        stack_pop(s, &o4);
        stack_pop(s, &o5);
        node_print(o1);
        node_print(o2);
        node_print(o3);
        node_print(o4);
        node_print(o5);
}*/
