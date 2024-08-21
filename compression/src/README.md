Luca Schram\
lschram\
Assignment 7: Huffman Coding Trees\

Assignment Overview:\
In this assignment we use the Huffman Coding lossless encoding/decoding algorithm in\
order to decrease the size of given files when encoding and to reconstruct given files\
when decoding.


Compilation Instructions:\
To compile both the executables, encode and decode, first confirm that all of the files\
listed below in the deliverables are present. Then, to compile both executables, simply\
type "make" in command line to compile both of them. If you only want to compile one or\
the other, then type "make encode" or "make decode" to compile encode or decode, respectively\
To format the code, type "make format". To perform scan-build on the code, type\
"make scan-build". To clean up the designated directory, type "make clean" to get rid of the\
object files and "make spotless" to get rid of the object files and both executables.



SCAN-BUILD FALSE POSITIVES:\
decode.c:100:7: warning: Access to field 'left' results in a dereference of a null pointer (loaded from variable 'current') [core.NullDereference]\
		if(current->left == NULL && current->right == NULL){\ 

This bug will never occur as the current node will always be reset to root before it reaches a NULL ptr. 




Encode Usage:\

SYNOPSIS\
  A Huffman encoder.\
  Compresses a file using the Huffman coding algorithm.\

USAGE\
  ./encode [-h] [-i infile] [-o outfile]\

OPTIONS\
  -h             Program usage and help.\
  -v             Print compression statistics.\
  -i infile      Input file to compress.\
  -o outfile     Output of compressed data.\



Decode Usage:\

SYNOPSIS\
  A Huffman decoder.\
  Decompresses a file using the Huffman coding algorithm.\

USAGE\
  ./huff-decode [-h] [-i infile] [-o outfile]\

OPTIONS\
  -h             Program usage and help.\
  -v             Print compression statistics.\
  -i infile      Input file to decompress.\
  -o outfile     Output of decompressed data.\



Deliverables:\

encode.c: Contains implementation of the Huffman encoder.\

decode.c: Contains implementation of the Huffman decoder.\

defines.h: Contains the macro definitions used throughout the assignment.\

header.h: Contains the struct definition for a file header.\

node.h: Contains the node ADT interface.\

node.c: Contains implementation of the node ADT.\

pq.h: Contains the priority queue ADT interface.\

pq.c: Contains implementation of the priority queue ADT.\

code.h: Contains the code ADT interface.\

code.h: Contains the code ADT interface.\

io.h: Contains the I/O module interface.\

io.c: Contains implementation of the I/O module.\

stack.h: Contains the stack ADT interface.\

stack.c: Contains implementation of the stack ADT.\

huffman.h: Contains the Huffman coding module interface.\

huffman.c: Contains implementation of the Huffman coding module interface.\

Makefile: File that will allow the grader to type make to compile my program.\

README.md: This file. Describes compilation instructions, deliverables, usage instructions for executables, and\
an assignment description.\

DESIGN.pdf: Describes the design goals, assignment overview, and pseudocode for this assignment. Also describes\
each files functions and those functions' arguments and return values.


