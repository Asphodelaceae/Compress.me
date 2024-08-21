#include "heap.h"
#include "node.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void swap(Node *x, Node *y) {
  // just swaps the pointers of two nodes in memory space, effectively swapping
  // them within an array as well
  Node t = *x;
  *x = *y;
  *y = t;
}

// just returns the parameter * 2 +1
uint32_t l_child(uint32_t n) { return (2 * n + 1); }

// returns parameter * 2 + 2
uint32_t r_child(uint32_t n) { return (2 * n + 2); }

// returns parameter -1 / 2
uint32_t parent(uint32_t n) { return ((n - 1) / 2); }

void up_heap(Node **arr, uint32_t n) {
  // basically loops through every node and every node's parent and checks if
  // its frequency value is < its parent. If it is, then swap them and set n =
  // the parent index of the node that was swapped around. This will basically
  // swap around parents and children until the min heap is refixed with respect
  // to the node that was tagged onto the bottom of the heap
  while ((n > 0) && (node_cmp(arr[n], arr[parent(n)]) == false)) {
    swap(arr[n], arr[parent(n)]);
    n = parent(n);
  }
}

void down_heap(Node **arr, uint32_t heap_size) {
  // point of this function is to take a heap with the root node popped off
  // and the bottom node of the array swapped to the root and go through the
  // heap and re-fix it so that it stays a valid heap. Assumes that its input
  // is already a near-valid heap.

  // initializes temp values for n and smaller
  uint32_t n = 0;
  uint32_t smaller;
  // continually loops through children until the index of the l_child is
  // greater than the heap size, which is the break condition
  while (l_child(n) < heap_size) {
    // sets smaller value to l_child index if the right
    // child is the same as the heap size
    if (r_child(n) == heap_size) {
      smaller = l_child(n);
    } else {
      // otherwise, check if the value at the index of l_child is
      // less than the one at r_child. If it is set smaller to
      // l_child, otherwise set it to r_child
      if (node_cmp(arr[l_child(n)], arr[r_child(n)]) == false) {
        smaller = l_child(n);
      } else {
        smaller = r_child(n);
      }
    }
    if (node_cmp(arr[n], arr[smaller]) == false) {
      // if the value at the current index is less than the one
      // at the 'smaller' index, break out of the while
      break;
    }
    swap(arr[smaller], arr[n]);
    // otherwise, swap the 'smaller' value and the current one to fix
    // the heap
    n = smaller;
    // set n to smaller
  }
}

/*
int main(void){
        Node *a = node_create('a', 2);
        Node *b = node_create('b', 1);
        swap(a, b);
        node_print(a);

        Node **node_arr = (Node **)calloc(5, sizeof(Node *));
        Node *one = node_create('r', 17);
        Node *two = node_create('e', 8);
        Node *three = node_create('x', 39);
        Node *four = node_create('z', 4);
        Node *five = node_create('m', 167);
        node_arr[0] = one;
        up_heap(node_arr, 0);
        node_arr[1] = two;
        up_heap(node_arr, 1);
        node_arr[2] = three;
        up_heap(node_arr, 2);
        node_arr[3] = four;
        up_heap(node_arr, 3);
        node_arr[4] = five;
        up_heap(node_arr, 4);
        //first node pop
        node_print(node_arr[0]);
        swap(node_arr[0], node_arr[4]);
        node_arr[4] = 0;
        down_heap(node_arr, 4);
        //second node pop
        node_print(node_arr[0]);
        swap(node_arr[0], node_arr[3]);
        node_arr[3] = 0;
        down_heap(node_arr, 3);
        //third node
        node_print(node_arr[0]);
        swap(node_arr[0], node_arr[2]);
        node_arr[2] = 0;
        down_heap(node_arr, 2);
        //fourth node
        node_print(node_arr[0]);
        swap(node_arr[0], node_arr[1]);
        node_arr[1] = 0;
        down_heap(node_arr, 1);
        // fifth node
        node_print(node_arr[0]);
        return 0;
}*/
