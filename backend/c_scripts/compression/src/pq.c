#include "pq.h"
#include "heap.h"
#include "node.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// struct for PQ. basically just contains the max capacity for the
// queue, the number of elements currently in it, and an array of
// nodes for the actual prio queue itself
typedef struct PriorityQueue {
  uint32_t capacity;
  uint32_t elems;
  Node **arr;
} PriorityQueue;

PriorityQueue *pq_create(uint32_t capacity) {
  // allocate space for the queue, and make sure it allocates properly
  // otherwise free it
  PriorityQueue *q = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  if (q) {
    // do the same as above for the array of nodes with a total
    // size of the capacity param passed in.
    // make sure it allocates properly as well, and then set
    // its max capacity var = capacity param and elems = 0 since
    // its empty as of now
    q->arr = (Node **)calloc(capacity, sizeof(Node *));
    if (q->arr) {
      q->capacity = capacity;
      q->elems = 0;
    } else {
      free(q->arr);
      q->arr = NULL;
      free(q);
      q = NULL;
      return (q);
    }
  } else {
    free(q);
    q = NULL;
  }
  // return the ptr to the queue
  return (q);
}

void pq_delete(PriorityQueue **q) {
  // check if the ptr is non null. If it is, then delete the node arr
  // if it is also non-null and set its ptr to NULL, then delete the
  // pq instance itself and set its prio to null
  if (*q) {
    if ((*q)->arr) {
      free((*q)->arr);
      (*q)->arr = NULL;
    }
    free(*q);
    (*q) = NULL;
  }
}

bool pq_empty(PriorityQueue *q) {
  // do same non-NULL check as always, then check if there are 0 elements
  // in the pq. If there are, return true, otherwise false
  if (q) {
    if (q->elems == 0) {
      return true;
    }
    return false;
  }
  return false;
}

bool pq_full(PriorityQueue *q) {
  // same thing as empty, but check if there are 'capacity' elements in the
  // pq, and return true if there are.
  if (q) {
    if (q->elems == q->capacity) {
      return true;
    }
    return false;
  }
  return false;
}

uint32_t pq_size(PriorityQueue *q) {
  // return number of elements in the prio q
  if (q) {
    return (q->elems);
  }
  return 0;
}

bool enqueue(PriorityQueue *q, Node *n) {
  // utilizes the heap implementation from assignment 4 contained in 'heap.c'
  // does basic non-NULL ptr checks, then checks if the queue is full before
  // trying to enqueue anything onto it.
  //
  // If it passes that, then set the last element of the current minheap array
  // to the passed in node and call up_heap to fix the heap in relation to that
  // new node. Then, just increment elems by 1 to indicate a new node is in the
  // queue, and return true to indicate success.
  if (q && n) {
    if (pq_full(q)) {
      return false;
    } else {
      q->arr[q->elems] = n;
      up_heap(q->arr, q->elems);
      q->elems += 1;
      return true;
    }
  } else {
    return false;
  }
}

bool dequeue(PriorityQueue *q, Node **n) {
  // same initial checks as enqueue, but checks if its empty instead
  if (q) {
    if (pq_empty(q)) {
      (*n) = NULL;
      return false;
    } else {
      // then, swap the last and first element in the array
      // so that you can pop off the "root" from the end of the
      // array instead of the beginning to prevent the need for shifting
      // the entire array over.
      //
      // Then, pop off that node by setting *n equal to it and setting
      // that array elem to 0. Then, decrement the size of elems by 1
      // to indicate one less node, and call down_heap to fix the min heap
      // starting from the root node and going down. This will move the new
      // "root" to its proper place in the heap, maintaining the minheap
      // invariant.
      swap(q->arr[0], q->arr[pq_size(q) - 1]);
      (*n) = q->arr[pq_size(q) - 1];
      q->arr[pq_size(q) - 1] = 0;
      q->elems -= 1;
      down_heap(q->arr, q->elems);
      return true;
    }
  } else {
    return false;
  }
}

void pq_print(PriorityQueue *q) {
  // just loop through every node in the node arr and print it. Only a partial
  // ordering, but i dont use it much so it doesnt matter.
  if (q) {
    for (uint32_t i = 0; i < pq_size(q); i += 1) {
      node_print(q->arr[i]);
    }
  }
}

/*
int main(void){
        PriorityQueue *q = pq_create(20);
        Node *one = node_create('r', 17);
        Node *two = node_create('e', 7);
        Node *three = node_create('x', 5);
        Node *four = node_create('m', 3);
        Node *five = node_create('i', 2);
        Node *a = NULL;
        Node *b = NULL;
        Node *c = NULL;
        Node *d = NULL;
        Node *e = NULL;
        enqueue(q, one);
        enqueue(q, two);
        enqueue(q, three);
        enqueue(q, four);
        enqueue(q, five);
        dequeue(q, &a);
        dequeue(q, &b);
        dequeue(q, &c);
        dequeue(q, &d);
        dequeue(q, &e);
        node_print(a);
        node_print(b);
        node_print(c);
        node_print(d);
        node_print(e);
        pq_delete(&q);
        return 0;
}*/
