#include <stdio.h>

#define MAXCODELENGTH 257
#define BYTESIZE 8

typedef struct Node {
	struct Node * left;
	struct Node * right;

	//ascii value of the character
	unsigned char val;
	unsigned frequency;
} Node;


typedef struct NodeArray {
	int nChars; //Number of elements in the array
	int capacity;
	Node ** nodes; //Array of nodes
} NodeArray;

unsigned char bit_buffer;
unsigned nBits;
char * char_to_code[256]; 



//Functions in huffman.c
//Creates histogram from file and fills up the node array
NodeArray * createHistogramFromFile(char * input_file, int ascii_freq[]); 
// void insertInNodeArray(int c, int freq, NodeArray* node_array);
// [Change for convienience] added left and right to init
void insertInNodeArray(int c, int freq, NodeArray* node_array, Node *left, Node *right);
void printNodeArray(NodeArray * node_array);
NodeArray * initializeNodeArray(int ascii_freq[], int nChars);
void sortNodeArray(NodeArray * node_array);
//Retrieves node with minimum frequency from array
Node * getMin(NodeArray * node_array); 
Node * getRootHuffmanTree(NodeArray * huffman_tree);
void printTree(Node * root, int code[], int i);
NodeArray * buildHuffmanTree(NodeArray * node_array);
void printHuffmanTree(NodeArray * huffman_tree);
int isLeafNode(Node * n);
void clearNodeArray(NodeArray * node_Array);
void codifyChars(NodeArray * huffman_tree);
void textToCode(Node * root, int code[], int i);

//Functions in compress.c
void createCompressedFile(char * input_file, char * output_file, NodeArray * node_array, NodeArray * huffman_tree);
void writeCharFrequenciesinFile(FILE * out_file, NodeArray * node_array);
void padBuffer(FILE * out_file);
void writeBitBuffer(int bit, FILE * out_file);
//Assumes that the next byte to read is the first byte to decode (just past the header)
void writeUncompressedFile(FILE* src, FILE* dest, NodeArray* huffman_tree);

//Functions in decompress.c
void uncompress(char * input_file, char* output_file);
char readCharFromFile(FILE * in_file);
int readIntFromFile(FILE * in_file);

NodeArray * buildDecodingHuffmanTree(NodeArray * node_array);
NodeArray * buildNodeArrayFromFile(FILE* in_file);


