
CC = gcc -std=gnu99 -g

all: huffman

huffman: compress.o decompress.o tree.o huffman.c
	$(CC) -o huffman huffman.c tree.o compress.o decompress.o
	
compress: compress.c
	$(CC) -c compress.c

tree: tree.c
	$(CC) -c tree.c

decompress: decompress.c
	$(CC) -c decompress.c
