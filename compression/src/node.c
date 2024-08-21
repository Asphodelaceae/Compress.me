#include "node.h"
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;

Node *node_create(uint8_t symbol, uint64_t frequency) {
  // allocate space for the new node and check if
  // it allocated properly. If it did, then just set the symbol and freq
  // to the passed in parameters and set left and right to null.
  // Otherwise just free it
  Node *n = (Node *)malloc(sizeof(Node));
  if (n) {
    n->symbol = symbol;
    n->frequency = frequency;
    n->left = NULL;
    n->right = NULL;
  } else {
    free(n);
    n = NULL;
  }
  return (n);
}

void node_delete(Node **n) {
  // check that the node ptr is non-null. If it is, then set all of the
  // node's information to 0 and NULL, free it, then set the ptr to NULL
  if (*n) {
    (*n)->symbol = 0;
    (*n)->frequency = 0;
    (*n)->left = NULL;
    (*n)->right = NULL;
    free(*n);
    (*n) = NULL;
  }
}

Node *node_join(Node *left, Node *right) {
  // make sure left and right are valid nodes, then create a node that with the
  // character
  // '$' and its frequency as the sum of the left and right frequency. Then,
  // make sure that the node allocated properly. If it didnt, free it and return
  // null. If it did, then link the left and right nodes to their new parent
  // node and return.
  if (left && right) {
    Node *joined =
        node_create((uint8_t)'$', left->frequency + right->frequency);
    if (!joined) {
      free(joined);
      joined = NULL;
      return (joined);
    }
    joined->left = left;
    joined->right = right;
    return (joined);
  } else {
    Node *joined = NULL;
    return (joined);
  }
}

void node_print(Node *n) {
  // make sure the node is non-null
  if (!n) {
    return;
  }
  // if the symbol isnt printable, use this weird thing from piazza that i dont
  // really know what it does or why its necessary, otherwise just print the
  // symbol
  if (iscntrl(n->symbol) != 0) {
    fprintf(stderr, "0x%02" PRIx8 "\n", n->symbol);
  } else {
    if (isprint(n->symbol) != 0) {
      fprintf(stderr, "char: %c\n", n->symbol);
    }
  }
  // finally just print the freq value
  fprintf(stderr, "freq: %lu\n", n->frequency);
}

bool node_cmp(Node *n, Node *m) {
  // if either of the nodes is null, return false immediately
  if (!n || !m) {
    return false;
  }
  // otherwise, check if the left freq is greater than the right. If it is then
  // return true, otherwise false
  if (n->frequency > m->frequency) {
    return true;
  }
  return false;
}

void node_print_sym(Node *n) {
  // same exact thing as node_print but only print the symbol and not the
  // frequency
  if (!n) {
    return;
  }
  if (iscntrl(n->symbol) != 0) {
    fprintf(stderr, "0x%02" PRIx8 "\n", n->symbol);
  } else {
    if (isprint(n->symbol) != 0) {
      fprintf(stderr, "char: %c\n", n->symbol);
    }
  }
}
