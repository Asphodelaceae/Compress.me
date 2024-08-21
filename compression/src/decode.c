#include "code.h"
#include "defines.h"
#include "header.h"
#include "huffman.h"
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
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define OPTIONS "hi:o:v"

int main(int argc, char **argv) {
  // initialize the operator and default values for the infile and outfile
  // as stdin and stdout
  // then initialize a bool for printing stats
  int operator;
  int inf = STDIN_FILENO;
  int outf = STDOUT_FILENO;
  bool stats = false;
  // parse options. If they give -h then print the help msg and return 0
  // immediately If they give -i, then set inf to the new opened infile using
  // RDONLY If they give -o, then set outf to the new outfile, and create it if
  // it doesnt exist. If they give -v, then set the stats bool to true so that
  // stats can be printed later.
  while ((operator= getopt(argc, argv, OPTIONS)) != -1) {
    switch (operator) {
    case 'h':
      fprintf(stderr, "SYNOPSIS\n");
      fprintf(stderr, "  A Huffman decoder.\n");
      fprintf(stderr,
              "  Decompresses a file using the Huffman coding algorithm.\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "USAGE\n");
      fprintf(stderr, "  ./decode [-h] [-i infile] [-o outfile]\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "OPTIONS\n");
      fprintf(stderr, "  -h             Program usage and help.\n");
      fprintf(stderr, "  -v             Print compression statistics.\n");
      fprintf(stderr, "  -i infile      Input file to decompress.\n");
      fprintf(stderr, "  -o outfile     Output of decompressed data.\n");
      return 0;
    case 'i':
      inf = open(optarg, O_RDONLY);
      if (inf == -1) {
        return 1;
      }
      break;
    case 'o':
      outf = open(optarg, O_CREAT | O_WRONLY);
      break;
    case 'v':
      stats = true;
      break;
    default:
      // print help msg on bad opt.
      fprintf(stderr, "SYNOPSIS\n");
      fprintf(stderr, "  A Huffman decoder.\n");
      fprintf(stderr,
              "  Decompresses a file using the Huffman coding algorithm.\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "USAGE\n");
      fprintf(stderr, "  ./decode [-h] [-i infile] [-o outfile]\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "OPTIONS\n");
      fprintf(stderr, "  -h             Program usage and help.\n");
      fprintf(stderr, "  -v             Print compression statistics.\n");
      fprintf(stderr, "  -i infile      Input file to decompress.\n");
      fprintf(stderr, "  -o outfile     Output of decompressed data.\n");
      return 1;
    }
  }
  // initialize a new header array that can just rip the info of the file's
  // header info into
  uint8_t head_buf[sizeof(Header)];
  // read all of the bytes for the header info into the header arr
  read_bytes(inf, head_buf, sizeof(Header));
  // initialize a new header instance and copy all of the info in the head_buf
  // array straight into the header instance using memcpy, basically just
  // completely recreating the original header's info
  Header new_head;
  memcpy(&new_head, head_buf, sizeof(Header));
  // check the magic number and return an error if its not the same.
  if (new_head.magic != MAGIC) {
    fprintf(stderr, "Invalid magic number.\n");
    return 1;
  }
  // set the outfiles perms = to the headers given permission bits.
  fchmod(outf, new_head.permissions);
  // make the tree array sized as the original header's tree size and then zero
  // it out using memset
  uint8_t tree[new_head.tree_size];
  memset(tree, 0, new_head.tree_size);
  // read the whole tree dump into the tree array
  read_bytes(inf, tree, new_head.tree_size);
  // rebuild the tree using the tree dump array
  Node *root = rebuild_tree(new_head.tree_size, tree);
  // set three temp vars, one for the current bit being looked at in the file,
  // one for the character being looked at as a result of the code, and one for
  // the current node.
  uint8_t bit = 0;
  uint8_t charbuf[1];
  Node *current = root;
  // loop while the number of bytes written to the outfile isnt equal to the
  // original file size, basically loop until they are equal.
  while (bytes_written != new_head.file_size) {
    // read a bit from the infile
    read_bit(inf, &bit);
    // traverse left if the bit is 0, traverse right if it is 1.
    if (bit == 0) {
      // go to the current nodes left node, and check if it is a leaf. If it is,
      // set the buffer's only index to the current nodes symbol and then write
      // it to the outfile, then reset the node to the root node
      current = current->left;
      if (current->left == NULL && current->right == NULL) {
        charbuf[0] = current->symbol;
        write_bytes(outf, charbuf, 1);
        current = root;
      }
    }
    // traverse right
    else {
      // same exact thing as above, just traveres to the current nodes right
      // node instead of left
      current = current->right;
      if (current->left == NULL && current->right == NULL) {
        charbuf[0] = current->symbol;
        write_bytes(outf, charbuf, 1);
        current = root;
      }
    }
  }
  // calculates stats using bytes read and bytes written which are reset at the
  // beginning of this file's call
  if (stats) {
    fprintf(stderr, "Compressed file size: %lu bytes\n", bytes_read);
    fprintf(stderr, "Decompressed file size: %lu bytes\n", bytes_written);
    float saved = 100 * (1 - ((float)bytes_read / (float)bytes_written));
    fprintf(stderr, "Space saving: %.2f%%\n", saved);
  }
  // delete the tree and close the infile and outfile
  delete_tree(&root);
  close(inf);
  close(outf);
  return 0;
}
