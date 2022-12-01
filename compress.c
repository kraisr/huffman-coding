//File for functions focused on compression/encoding
#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"


// Writes the number of unique characters to the file as a single byte
// Then, for each character in the node array, write the character (as a byte)
// and the frequency of that character (size = sizeof(unsigned int))
// Format: [nChars] [char1 ascii code(char)] [char1 frequency(int)] [char2 ascii code] [char2 frequency] ...
void writeCharFrequenciesinFile(FILE * out_file, NodeArray * node_array){
	//TODO: write your implementation here
	int i = 0;
	// Writing the first bit [nChars]
	bit_buffer = node_array->nChars;
	// Changing numBits to 7 to the current bit_buffer immediately
	nBits = 7;
	writeBitBuffer(0, out_file);
	int k = -1;

	while (i < node_array->nChars) {
		if (k == 0) {
			// Adding the (ascii) val of the char to the bit_buffer
			bit_buffer = (int) node_array->nodes[i]->val;
			// Since this is the only bit add 7 0's and push it
			nBits = 7;
			writeBitBuffer(0, out_file);
			k++;
		} else if (k == 1) {
			// Adding the (int) frequency of the char to the bit buffer
			bit_buffer = node_array->nodes[i]->frequency;
			// Since this is the only bit add 7 0's and push it
			nBits = 7;
			writeBitBuffer(0, out_file);
			i++;
			k++;
		} else {
			// if we have already written the [nChar (ascii)][nChar(frequency)]
			// then write the 3 bytes of 0's seperating them
			for (int x = 0; x < 3; x++) {
				writeBitBuffer(0, out_file);
				padBuffer(out_file);
			}
			k = 0;
		}
		// Accounting for the last partial byte
		if (i == node_array->nChars) {
			for (int x = 0; x < 3; x++) {
				writeBitBuffer(0, out_file);
				padBuffer(out_file);
			}
		}
	}
}

// Adds 0 bits to the end of the final byte if the encoded document
// ends with a partial byte (cannot write single bits to the file!)
void padBuffer(FILE * out_file){
	//TODO: write your implementation here
	while (nBits > 0) {
		// Padding the buffer with 0's for partial bits
		writeBitBuffer(0, out_file);
	}
}

// Add the next bit to the bit buffer. If a full byte is buffered, 
// write it to the file (cannot write individual bits to a file!)
// Reset nBits and the bitBuffer if the buffer is written to the file
void writeBitBuffer(int bit, FILE * out_file){
	//TODO: write your implementation here
	unsigned char mask;
	if (bit == 1) {
		mask = 1;
		mask = mask << (7 - nBits);
		bit_buffer = bit_buffer | mask;
	}
	nBits++;
	// Once bits reach 8 in the buffer write them to the file
	if (nBits == 8) {
		fwrite(&bit_buffer, sizeof(unsigned char), 1, out_file);
		nBits = 0;
		bit_buffer = 0;
	}
}


// Given a code string, it takes each "bit" representing character and
// writes it to the bit buffer for the output file
void writeCodeToFile (char * code, FILE * out_file){
	//TODO: write your implementation here
	char * temp = code;
	while (*temp != '\0') {
		int c = (int) *temp - '0';
		writeBitBuffer(c, out_file);
		temp++;
	}

}


// Primary compression method
// (1) stores frequencies in the output file
// (2) rewrites the input file using the Huffman Code in the huffman_tree
void createCompressedFile(char * input_file, char * output_file, NodeArray * node_array, NodeArray * huffman_tree){
	//TODO: write your implementation here
	FILE *in_file = fopen(input_file, "r"),
		*out_file = fopen(output_file, "wb");
	// writing the header
	writeCharFrequenciesinFile(out_file, node_array);
	// while (nBits > 0) {
	// 	writeBitBuffer(0, out_file);
	// }
	int c;
	// Iterating through file char by char and assigning char_to_code
	while ((c=fgetc(in_file)) != EOF) {
		writeCodeToFile(char_to_code[c], out_file);
	}
	// Checking for the remaining bits that may need padding after the file ended
	while (nBits > 0) {
		writeBitBuffer(0, out_file);
	}
}
