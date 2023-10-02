//File for functions focused on decompression/decoding
#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

// Read and return a single int from the file
int readIntFromFile(FILE * in_file){
	return (int) (fgetc(in_file) - '0');
}


// Read and return a single char from the file
char readCharFromFile(FILE * in_file){
	return (char)fgetc(in_file);
}

// Helper function to print current bits for testing
void printBits(unsigned char temp) {
	// Since we know usnigned char can store 1 byte which is 8 bits we can hard code 8 iterations for each bit
	for (int testedBit = 7; testedBit >= 0; testedBit--) {
		// Getting a single bit at index testedBit (reverse order to since the bits are reversed)
		int bit = (temp >> testedBit) & 1;
			if (bit) {
				printf("1");
			} else {
				printf("0");
			}
	}
	printf("\n");
}

// Read and return the number of unique characters in the file
// Obtain all character frequencies of the encoded file and store them in ascii_freq
int loadFrequenciesFromFile(FILE * in_file, int* ascii_freq){
	char c = fgetc(in_file);
	int nChars = (int) c;
	int temp = nChars;
	char lastChar;
	int i = 1;
	while ((c=fgetc(in_file)) != EOF && temp) {
		// Using the pattern that there are 3 bits of 0's seperating [nChar (ascii)][nChar (freq)]
		// That means that every 5th bit will be the ascii value and every 5th bit + 1 is the freq
		// Ommit first 3 bits since they are the num of unique chars and the seperating 0's
		if (i > 3 && (i + 1) % 5 == 0) {
			// Storing the last seen char
			lastChar = c;
		} else if (i > 3 && (i + 1) % 5 == 1) {
			// Assigning last seen char to its frequency
			ascii_freq[lastChar] = (unsigned char) c;
			temp--;
		}
		i++;
	}
	return nChars;
}


// Allocates, initializes, and returns a node array from the file
// (1) Load all frequency information from the header of the specified file
// (2) Builds a node array from the file frequencies
NodeArray * buildNodeArrayFromFile(FILE* in_file){
	unsigned ascii_freq[256] = {0};
	int nChars = loadFrequenciesFromFile(in_file, ascii_freq);
	NodeArray *node_array = initializeNodeArray(ascii_freq, nChars);
}

// Decodes the input file using the huffman tree
// Write the decoded file into the output file
// Assumes that the next byte to read is the first byte to decode (just past the header)
void writeUncompressedFile(FILE* in_file, FILE* out_file, NodeArray* huffman_tree){
	FILE *temp = in_file;

	Node *root = getRootHuffmanTree(huffman_tree);
	int nChars = huffman_tree->nChars + 1;
	unsigned char c;
	int freq = root->frequency;
	int written_chars = 0;
	/*
	This is to keep track of the header. The header will always be nChars * 6
	Using that fact we can track when we get to the body of the encrypted file
	Structure is: (header) nChars * 6, and rest is body
	*/ 
	int i = nChars * 6;
	// This formula finds the beginning of the body.
	int target = nChars * 6 - 1 - (2 * nChars) - (3 * (nChars + 1));

	while (1) {
		c = fgetc(in_file);
		
		unsigned char temp = c;
		// printBits(temp);
		if (i <= target && written_chars < freq) {
			for (int testedBit = 7; testedBit >= 0; testedBit--) {
				// Getting bit at index testedBit
				int bit = (temp >> testedBit) & 1;
				if (bit == 1) {
					// if bit is 1 then go left in the tree
					root = root->left;
				} else {
					// else go right
					root = root->right;
				}
				if (isLeafNode(root)) {
					// Once we get to leaf node, write it to the decrypted file
					fprintf(out_file, "%c", root->val);
					// Reset the root
					root = getRootHuffmanTree(huffman_tree);
					// Increment the amount of chars written
					written_chars++;
					// Check if we have all the chars
					if (written_chars >= freq) {
						break;
					}
				}
			}
		}
		// Check if we have all the chars
		if (written_chars >= freq) {
			break;
		}
		i--;
	}
}

// Primary decoding function: 
// (1) Open the input file and load all frequency information
// (2) Build the Huffman Tree for those frequencies
// (3) Traverse the Huffman Tree bit-by-bit and write each decoded
// character to the output file
// (4) Free the node arrays and close the files
void uncompress(char * input_file, char* output_file){
	// Open the files
	FILE *in_file = fopen(input_file, "r"),
		*out_file = fopen(output_file, "wb");
	// Create node_array
	NodeArray *node_array = buildNodeArrayFromFile(in_file);
	// Reset the pointer for then input_file (it was iterated when creating node_array)
	in_file = fopen(input_file, "r");
	// Build huffman_tree
	NodeArray *huffman_tree = buildHuffmanTree(node_array);
	// Decompress the file and write it to out
	writeUncompressedFile(in_file, out_file, huffman_tree);

}
