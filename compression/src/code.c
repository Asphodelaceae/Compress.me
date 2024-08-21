#include "code.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Code;

Code code_init(void) {
  // just makes a new code on the stack, zeroes out its internal array, and sets
  // top to 0.
  Code c;
  memset(c.bits, 0, MAX_CODE_SIZE);
  c.top = 0;
  return (c);
}

uint32_t code_size(Code *c) {
  // returns top, which is the size of the code since itll increment
  // with each pushed bit
  if (c) {
    return (c->top);
  }
  return 0;
}

bool code_empty(Code *c) {
  // checks if top is 0, meaning no bits in the stack,
  // and returns true if so, otherwise false
  if (c) {
    if (c->top == 0) {
      return true;
    }
  }
  return false;
}

bool code_full(Code *c) {
  // checks if top is 255, meaning max bits on the stack,
  // and returns true if so, otherwise false
  if (c) {
    if (c->top == ALPHABET) {
      return true;
    }
  }
  return false;
}

bool code_set_bit(Code *c, uint32_t i) {
  // same exact thing as in bitvector from asgn6. Basically just
  // takes a given position for the bit, makes sure its in the range
  // of the bitstack size, and then sets it by calculating
  // its bit and byte positions using / and %, then shifts it using
  // same formula and returns true to indicate success
  if (c) {
    if (i > code_size(c)) {
      return false;
    }
    uint32_t bytepos = i / 8;
    uint32_t bitpos = i % 8;
    c->bits[bytepos] |= 1UL << bitpos;
    return true;
  }
  return false;
}

bool code_clr_bit(Code *c, uint32_t i) {
  // exact same as set bit, but diff operation to clear the bit instead
  // of setting it. makes a bitmask to clear it.
  if (c) {
    if (i > code_size(c)) {
      return false;
    }
    uint32_t bytepos = i / 8;
    uint32_t bitpos = i % 8;
    c->bits[bytepos] &= ~(1UL << bitpos);
    return true;
  }
  return false;
}

bool code_get_bit(Code *c, uint32_t i) {
  // same thing as bitvector again. Makes a temporary value from the current
  // byte, sets the byte, and checks if it changed. If it didnt, it was a 1
  // at that spot, so return true. If it did change, it was a 0, so return
  // false.
  if (c) {
    if (i > code_size(c)) {
      return false;
    }
    uint32_t bytepos = i / 8;
    uint32_t bitpos = i % 8;
    uint8_t temp = c->bits[bytepos];
    temp |= 1UL << bitpos;
    if (temp == c->bits[bytepos]) {
      return true;
    }
  }
  return false;
}

bool code_push_bit(Code *c, uint8_t bit) {
  // pushes a bit onto the stack after making sure the stack isnt full
  // by setting the bit at position "top" to 1 or 0 depending on the value
  // of the passed in parameter "bit". Returns true to indicate success
  if (c) {
    if (code_full(c)) {
      return false;
    } else if (bit) {
      code_set_bit(c, c->top);
    } else if (!bit) {
      code_clr_bit(c, c->top);
    }
    c->top += 1;
    return true;
  }
  return false;
}

bool code_pop_bit(Code *c, uint8_t *bit) {
  // same thing as push, but rips off the bit on the end of the stack instead.
  // Have to do top - 1 since top is the number of elements and we want the
  // actual index of the bit. If get bit outputs true, set bit to 1, otherwise
  // set it to 0. Finally, clear the bit at that index to "remove" it from the
  // stack.
  if (c) {
    if (code_empty(c)) {
      return false;
    } else {
      bool bit_out = code_get_bit(c, c->top - 1);
      if (bit_out) {
        *bit = 1;
      } else {
        *bit = 0;
      }
      code_clr_bit(c, c->top - 1);
      c->top -= 1;
      return true;
    }
  }
  return false;
}

void code_print(Code *c) {
  // just loops through the stack from index 0 and prints the bit.
  if (c != 0 && c != NULL) {
    for (uint32_t i = 0; i < code_size(c); i += 1) {
      printf("%d", code_get_bit(c, i));
    }
    printf("\n");
  }
}

/*
int main(void){
        Code co = (code_init());
        Code *c = (&co);
        code_push_bit(c, 1);
        code_push_bit(c, 1);
        code_push_bit(c, 0);
        code_push_bit(c, 1);
        code_print(c);
        uint8_t bit = 0;
        code_pop_bit(c, &bit);
        printf("bit:%u\n", bit);
        code_pop_bit(c, &bit);
        printf("bit:%u\n", bit);
        code_print(c);
        code_pop_bit(c, &bit);
        printf("bit:%u\n", bit);
        code_push_bit(c,1);
        code_pop_bit(c, &bit);
        printf("bit:%u\n", bit);
        code_print(c);
}*/
