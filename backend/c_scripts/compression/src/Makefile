CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra -Ofast -gdwarf-4

all: encode decode

.PHONY: clean spotless all main format

encode: heap.o node.o pq.o code.o stack.o io.o huffman.o encode.o
	$(CC) -o $@ $^

decode: heap.o node.o pq.o code.o stack.o io.o huffman.o decode.o
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o

spotless:
	rm -f encode decode *.o

format:
	clang-format -i -style=file *.[c]

scan-build: clean
	scan-build --use-cc=$(CC) make
