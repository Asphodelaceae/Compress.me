#include "code.h"
#include "defines.h"
#include "io.h"
#include "node.h"
#include "pq.h"
#include "stack.h"
#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Node *build_tree(uint64_t hist[static ALPHABET]) {
  // make the priority queue and fill it with all the ascii values within the
  // histogram with the ascii value as its symbol and the frequency as the value
  // in hist[ascii value]
  PriorityQueue *pq = pq_create(ALPHABET);
  for (uint32_t i = 0; i < ALPHABET; i += 1) {
    if (hist[i] != 0) {
      Node *new = node_create(i, hist[i]);
      enqueue(pq, new);
    }
  }
  // priority queue has been fully built at this point,
  // so move on to the actual tree building.
  // Pseudo provided for us, but basically just loops until theres only 1 node
  // in the prio queue, dequeuing two nodes, joining them, and enqueuing their
  // parent node as an interior node
  while (pq_size(pq) > 1) {
    Node *left = NULL;
    Node *right = NULL;
    dequeue(pq, &left);
    dequeue(pq, &right);
    Node *parent = node_join(left, right);
    enqueue(pq, parent);
  }
  // finally, make the root node ptr, dequeue the final node, and delete the
  // prio queue and return the root
  Node *root = NULL;
  dequeue(pq, &root);
  pq_delete(&pq);
  return (root);
}

static void sub_build(Node *root, Code table[static ALPHABET], Code *c) {
  // wrapper function for build codes so that i dont have to deal with the
  // janky static declaration of code and can instead just pass its address
  // in to the recursive function after declaring it in the initial function.
  // Just continaully recurses until it finds a leaf node and sets the symbol's
  // code value equal to the current instance of the function scope code.
  // Otherwise, just pushes and pops bits to define the recursion down the tree.
  if (root) {
    if (root->left == NULL && root->right == NULL) {
      table[root->symbol] = *c;
    } else {
      uint8_t bit;
      code_push_bit(c, 0);
      sub_build(root->left, table, c);
      code_pop_bit(c, &bit);

      code_push_bit(c, 1);
      sub_build(root->right, table, c);
      code_pop_bit(c, &bit);
    }
  }
}

void build_codes(Node *root, Code table[static ALPHABET]) {
  // calls the recursive wrapper for build_codes after
  // initializing the bitstack code.
  Code bitstack = code_init();
  sub_build(root, table, &bitstack);
}

void dump_tree(int outfile, Node *root) {
  // pseudo also provided to us. Basically just performs a post order traversal
  // of the tree. When it encounters as interior node, it writes out an "I" to
  // indicate an interior node. If it encounters a leaf, writes out "L" followed
  // by the leafs symbol to indicate that its a leaf node with the symbol
  // "root->symbol".
  if (root) {
    dump_tree(outfile, root->left);
    dump_tree(outfile, root->right);
    if (root->left == NULL && root->right == NULL) {
      uint8_t buf[2];
      buf[0] = 'L';
      write_bytes(outfile, &buf[0], 1);
      buf[1] = root->symbol;
      write_bytes(outfile, &buf[1], 1);
    } else {
      uint8_t interior[1];
      interior[0] = 'I';
      write_bytes(outfile, &interior[0], 1);
    }
  }
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
  // will create a stack that contains all the nodes from the tree array that
  // has the postorder traversed, dumped tree information within it. It will
  // loop through the entire tree array and check each symbol. If it finds an
  // 'L', it will create a node with the symbol of the character following that
  // 'L' and push it onto the stack. It will then manually increment i by 1 to
  // skip looking at the symbol that followed the 'L' that was just pushed to
  // the stack. If it finds an interior node symbol 'I', it will pop two nodes
  // off the stack and set the equal to left and right ptrs. Then, it will join
  // those two nodes and push that new parent onto the stack. It will not
  // manually increment I so that it does not skip any chars in the tree arr.
  Stack *treestack = stack_create(nbytes);
  for (uint16_t i = 0; i < nbytes; i += 1) {
    if (tree[i] == 'L') {
      Node *leaf = node_create(tree[i + 1], 1);
      stack_push(treestack, leaf);
      i += 1;
    } else if (tree[i] == 'I') {
      Node *right = NULL;
      stack_pop(treestack, &right);
      Node *left = NULL;
      stack_pop(treestack, &left);
      Node *parent = node_join(left, right);
      stack_push(treestack, parent);
    }
  }
  // finally, once the entire tree has been created and there is only one node
  // left in the stack, pop it and set the root ptr = to it, as this is the root
  // of the reconstructed tree. Then delete the stack instance
  Node *root = NULL;
  stack_pop(treestack, &root);
  stack_delete(&treestack);
  return root;
}

void delete_tree(Node **root) {
  // basic post order traversal of the tree that will traverse down the tree
  // until it encounters a root node and then delete that root node, which will
  // delete the tree from the bottom up to maintain links to each of the nodes,
  // making sure they are deleted.
  if (*root) {
    delete_tree(&((*root)->left));
    delete_tree(&((*root)->right));
    node_delete(root);
  }
}

/*
int main(void){
        Code table[ALPHABET];
        memset(table, 0, ALPHABET * sizeof(Code));
        uint64_t hist[ALPHABET];
        memset(hist, 0, ALPHABET * 8);
        hist[67] = 7;
        hist[83] = 4;
        hist[98] = 2;
        hist[107] = 9;
        Node *root = build_tree(hist);
        build_codes(root, table);
        int opath = open("out.txt", O_WRONLY);
        dump_tree(opath, root);
        delete_tree(&root);
        close(opath);
        opath = open("out.txt", O_RDONLY);
        uint8_t tree[bytes_written];
        read_bytes(opath, tree, bytes_written);
        for(uint32_t i = 0; i < bytes_written; i+=1){
                printf("data: %u\n", tree[i]);
        }
        Node *bruh = rebuild_tree(bytes_written, tree);
        node_print(bruh);
        delete_tree(&bruh);

}*/
