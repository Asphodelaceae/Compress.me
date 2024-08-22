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
      fprintf(stderr, "  A Huffman encoder.\n");
      fprintf(stderr,
              "  Compresses a file using the Huffman coding algorithm.\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "USAGE\n");
      fprintf(stderr, "  ./encode [-h] [-i infile] [-o outfile]\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "OPTIONS\n");
      fprintf(stderr, "  -h             Program usage and help.\n");
      fprintf(stderr, "  -v             Print compression statistics.\n");
      fprintf(stderr, "  -i infile      Input file to compress.\n");
      fprintf(stderr, "  -o outfile     Output of compressed data.\n");
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
      fprintf(stderr, "  A Huffman encoder.\n");
      fprintf(stderr,
              "  Compresses a file using the Huffman coding algorithm.\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "USAGE\n");
      fprintf(stderr, "  ./encode [-h] [-i infile] [-o outfile]\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "OPTIONS\n");
      fprintf(stderr, "  -h             Program usage and help.\n");
      fprintf(stderr, "  -v             Print compression statistics.\n");
      fprintf(stderr, "  -i infile      Input file to compress.\n");
      fprintf(stderr, "  -o outfile     Output of compressed data.\n");
      return 1;
    }
  }
  // if they try to give a stdin input, then gotta do some janky stuff since
  // stdin cant be rewound. basically, just write everything in stdin to a file
  // that is newly created with both read and write capabilities, and that wipes
  // itself when its opened. Then, after you've written everything in stdin to
  // that new temporary file, rewind its file pointer to the beginning so it can
  // be used later.
  if (inf == STDIN_FILENO) {
    inf = open("tmpfile.txt", O_TRUNC | O_RDWR | O_CREAT);
    int bruh = 1;
    uint8_t tmpbuf[BLOCK];
    while (bruh != 0) {
      bruh = read_bytes(STDIN_FILENO, tmpbuf, BLOCK);
      if (bruh != 0) {
        write_bytes(inf, tmpbuf, bruh);
      }
    }
    lseek(inf, 0, SEEK_SET);
  }
  // reset bytes written and read to account for STDIN copying to a new file
  bytes_written = 0;
  bytes_read = 0;
  // construct the histogram by looping through each byte of the infile
  // individually and incrementing the value at its corresponding ascii value by
  // 1 each time its encountered.
  uint64_t hist[ALPHABET];
  uint8_t charbuf[1];
  int out = 1;
  memset(hist, 0, ALPHABET);
  while (out != 0) {
    out = read_bytes(inf, charbuf, 1);
    if (out == 1) {
      hist[charbuf[0]] += 1;
    }
  }
  // weird thing given by the lab document that makes sure huffman tree doesnt
  // break when given nothing.
  if (hist[0] == 0) {
    hist[0] = 1;
  }
  if (hist[1] == 0) {
    hist[1] = 1;
  }
  // builds the tree and gets the trees root.
  Node *root = build_tree(hist);
  // create the code table and initialize all of the codes inside of it to
  // prevent segfaults
  Code table[ALPHABET];
  for (uint32_t i = 0; i < ALPHABET; i += 1) {
    table[i] = code_init();
  }
  // then, call build codes to make all of the codes for the huffman tree
  build_codes(root, table);
  // header stuff, basically initialize the header and then set its info
  Header head;
  // set magic number to magic macro provided
  head.magic = MAGIC;
  // make the stat struct to pass into fstat, this will contain file info to be
  // accessed later
  struct stat f_info;
  fstat(inf, &f_info);
  // set the permissions equal to the perm bits inside the stat struct.
  head.permissions = f_info.st_mode;
  // set the outfiles permissions equal to the infile's permissions (idk why
  // lol)
  fchmod(outf, f_info.st_mode);
  // loop through the histogram and count the unique characters and set the tree
  // size equal to that * 3 - 1
  int count = 0;
  for (uint32_t i = 0; i < ALPHABET; i += 1) {
    if (hist[i] != 0) {
      count += 1;
    }
  }
  head.tree_size = (count * 3) - 1;
  // set file size equal to the stat structs stored filesize
  head.file_size = f_info.st_size;
  // this is super bizarre, but basically make a uint8_t array that can store
  // all of the bytes inside of the Header instance, and then copy over
  // literally everything from the header into that buffer and write it out
  // using write_bytes
  uint8_t headbuf[sizeof(Header)];
  memcpy(headbuf, &head, sizeof(Header));
  write_bytes(outf, headbuf, sizeof(Header));
  // finally, dump the tree to the file and delete the tree
  dump_tree(outf, root);
  delete_tree(&root);
  // rewind the infile's file ptr to the beginning, and then emit the code for
  // every characters ASCII value to the outfile.
  lseek(inf, 0, SEEK_SET);

  out = 1;
  while (out != 0) {
    out = read_bytes(inf, charbuf, 1);
    if (out == 1) {
      write_code(outf, &table[charbuf[0]]);
    }
  }
  // flush the remaining codes that werent written out
  flush_codes(outf);

  // output stats if they gave -v, which is basically just the filesize (could
  // be bytes_read instead of file_size) the number of bytes written which
  // indicates compressed file size, and the space saving given by the formula
  // in the lab doc
  if (stats) {
    fprintf(stderr, "Uncompressed file size: %lu bytes\n", head.file_size);
    fprintf(stderr, "Compressed file size: %lu bytes\n", bytes_written);
    float saved = 100 * (1 - ((float)bytes_written / (float)head.file_size));
    fprintf(stderr, "Space saving: %.2f%%\n", saved);
  }
  // close the in and outfiles and return 0 to indicate successful run.
  close(inf);
  close(outf);
  return 0;
}
