#include "io.h"
#include "code.h"
#include "defines.h"
#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// initialize two externs and the 6 static vars used for
// the rest of the functions
uint64_t bytes_read;
uint64_t bytes_written;
static uint8_t statbuf[BLOCK];
static uint32_t current = 0;
static uint32_t blocksize = 0;

static uint8_t codeblock[BLOCK];
static uint32_t ind = 0;
static uint32_t codesize = 0;

int read_bytes(int infile, uint8_t *buf, int nbytes) {
  // reads from the infile byte by byte until read returns 0,
  // indicating there are no more bytes to read from it, all while
  // incrementing the extern of how many bytes were read and then
  // return the total number of bytes read within this function call.
  int total_bytes = 0;
  int temp = 0;
  while (nbytes > total_bytes) {
    temp = read(infile, buf + total_bytes, 1);
    if (temp != 0) {
      bytes_read += 1;
      total_bytes += 1;
    } else {
      // no more bytes to read from the file
      break;
    }
  }
  return (total_bytes);
}

int write_bytes(int outfile, uint8_t *buf, int nbytes) {
  // exact same as read_bytes, but using the write() command instead
  // of read()
  int total_bytes = 0;
  int temp = 0;
  while (nbytes > total_bytes) {
    temp = write(outfile, buf + total_bytes, 1);
    if (temp != 0) {
      bytes_written += 1;
      total_bytes += 1;
    } else {
      // no more bytes to write from the buffer
      break;
    }
  }
  return (total_bytes);
}

bool read_bit(int infile, uint8_t *bit) {
  // read_bit is a lot like next word. It will be continually called with a
  // static index maintaining its position in the file / buffer array it is
  // writing to, and only read in a new block of info to read bits from when it
  // has run past the size of the block. Basically just continually looks at the
  // info within the read in block from read_bytes bit by bit, returning either
  // a 0 or a 1 depending on the current bit its on in the buffer array.
  //
  // It returns false if and only if read_bytes returns 0, indicating that its
  // reached EOF
  if (current >= blocksize * 8) {
    uint32_t out = 0;
    memset(statbuf, 0, BLOCK);
    out = read_bytes(infile, statbuf, BLOCK);
    if (out == 0) {
      current = 0;
      blocksize = 0;
      return false;
    } else {
      blocksize = out;
      current = 0;
    }
  }
  uint32_t bytepos = current / 8;
  uint32_t bitpos = current % 8;
  uint8_t temp = statbuf[bytepos];
  temp |= 1U << (bitpos);
  if (temp == statbuf[bytepos]) {
    *bit = 1;
    current += 1;
    return true;
  } else {
    *bit = 0;
    current += 1;
    return true;
  }
}

void write_code(int outfile, Code *c) {
  // pretty simple despite my million iterations of this function.
  // Basically just loops from index 0 to index "top" of the current
  // code and writes out the corresponding bit of the code to the static
  // buffer "codeblock". This will happen for each code, but this function
  // will only write them out itself if the static buffer is filled. Otherwise
  // it relies on flush_codes to write them out for it.
  if (c) {
    ind = 0;
    while (ind < code_size(c)) {
      uint8_t cur_bit = 0;
      cur_bit = code_get_bit(c, ind);
      uint32_t bytepos = (codesize / 8);
      uint32_t bitpos = (codesize % 8);
      if (cur_bit == 1) {
        codeblock[bytepos] |= 1U << (bitpos);
      } else {
        codeblock[bytepos] &= ~(1UL << bitpos);
      }
      codesize += 1;
      ind += 1;
      if ((codesize) / 8 >= BLOCK) {
        write_bytes(outfile, codeblock, BLOCK);
        memset(codeblock, 0, BLOCK);
        codesize = 0;
      }
    }
  }
}

void flush_codes(int outfile) {
  // will write out the remaining codes in codeblock. However,
  // since the block will not always line up evenly, if the number of bits that
  // have been written is not evenly divisible by 8, you have to write out an
  // extra byte since there are codes in that final, unfinished byte in the
  // buffer. Otherwise just call it normally since the byte array was filled
  // evenly.
  if (codesize % 8 == 0) {
    write_bytes(outfile, codeblock, (codesize / 8));
  } else {
    write_bytes(outfile, codeblock, (codesize / 8) + 1);
  }
}

/*
int main(void){
        //int fpath = open("pain.txt", O_RDONLY);
        int opath = open("out.txt", O_WRONLY);

        Code co = (code_init());
        Code *c = (&co);
        //ascii b
        code_push_bit(c, 0);
        code_push_bit(c, 1);
        code_push_bit(c, 1);
        code_push_bit(c, 0);
        code_push_bit(c, 0);
        code_push_bit(c, 0);
        code_push_bit(c, 1);
        code_push_bit(c, 0);

        code_push_bit(c, 0);
        code_push_bit(c, 0);
        code_push_bit(c, 1);
        code_push_bit(c, 0);
        code_push_bit(c, 0);
        code_push_bit(c, 0);
        code_push_bit(c, 0);
        code_push_bit(c, 0);

        for(int i = 0; i < 2049; i+=1){
                write_code(opath, c);
        }
        flush_codes(opath);
        //
        uint32_t size = 10000;
        uint8_t *buf = (uint8_t *)calloc(size, sizeof(uint8_t));
        uint8_t bit = 0;
        uint32_t i = 0;
        while(read_bit(fpath, &bit)){
                uint32_t bytepos = i / 8;
                uint32_t bitpos = i % 8;
                if(bit == 1){
                        buf[bytepos] |= 1U << (bitpos);
                }
                else{
                        buf[bytepos] &= ~(1UL << bitpos);
                }
                i += 1;
        }
        write_bytes(opath, buf, (i/8));
        //
}*/
