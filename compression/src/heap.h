#pragma once

#include "node.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

void swap(Node *x, Node *y);

uint32_t l_child(uint32_t n);

uint32_t r_child(uint32_t n);

uint32_t parent(uint32_t n);

void up_heap(Node **arr, uint32_t n);

void down_heap(Node **arr, uint32_t heap_size);
