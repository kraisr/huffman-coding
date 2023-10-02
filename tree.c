//File for functions focused on building a Huffman Tree
#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

//Free memory allocated to the node array
void clearNodeArray(NodeArray * node_array){
	free(node_array);
}

// Checks to see if nChars == capacity in the node_array
// If so, the size of the node_array is doubled
void resizeArrayIfNeeded(NodeArray * node_array){
	// Checking if the array has reached its max capacity
	if (node_array->nChars == node_array->capacity) {
		// Reallocating the memory for the node_array with double the size
		node_array = realloc(node_array, (2 * node_array->capacity) * sizeof(NodeArray));
		// Doubling the capacity accordingly
		node_array->capacity = 2 * node_array->capacity;
	}
}

// First calls resizeArrayIfNeeded
// Then, inserts a new node at the end of the node array for the new character
// All values for the node are initialized, and the nChars count is updated
void insertInNodeArray(int c, int freq, NodeArray * node_array, Node *left, Node *right){
	// Calling resize in case the capacity is too small
	resizeArrayIfNeeded(node_array);
	// Shifting i to the last position in the NodeArray
	int i = 0;
	for (i = 0; i < node_array->nChars; i++);
	// Creating a new node to insert at the end
	Node *newNode = (Node *) malloc(sizeof(Node));
	newNode->val = c;
	newNode->frequency = freq;
	// Inserting the new node
	node_array->nodes[i] = newNode;
	node_array->nChars++;
	// Assigning left and right child
	node_array->nodes[i]->left = left;
	node_array->nodes[i]->right = right;
}

// Node comparison function
// Compares two nodes by frequency
//
// Returns the difference a->freq - b->freq if it is non-zero
// Else, returns the difference a->val - b->val
int compNodes (const Node *nodeA, const Node *nodeB){
	if (nodeA->frequency - nodeB->frequency != 0) {
		return nodeA->frequency - nodeB->frequency;
	} else {
		return nodeA->val - nodeB->val;
	}
}

// Ascending sorting function for the nodes of the node array
// Sorts in ascending order by frequency 
// (uses minimum ascii value as a tie breaker)
// Hint: use the function compNodes to simplify your function
void sortNodeArray(NodeArray * node_array){
	for (int i = 0; i < node_array->nChars; i++) {
		for (int j = 0; j < node_array->nChars; j++) {
			if (compNodes(node_array->nodes[i], node_array->nodes[j]) < 0) {
				Node *temp = node_array->nodes[i];
				node_array->nodes[i] = node_array->nodes[j];
				node_array->nodes[j] = temp;
			}
		}
	}
}

// Allocates, initializes, and returns a node array
NodeArray * initializeNodeArray(int * ascii_freq, int nChars){
	NodeArray *nArray = (NodeArray *) malloc(nChars * sizeof(NodeArray));
	nArray->nChars = 0;
	nArray->capacity = nChars;
	nArray->nodes = (Node**) malloc(nArray->capacity * sizeof(Node)); 
	
	for (int i = 0; i < 256; i++) {
		if (ascii_freq[i] != 0) {
			insertInNodeArray(i, ascii_freq[i], nArray, NULL, NULL);
		}
	}

	sortNodeArray(nArray);
	return nArray;
}

// Opens the specified file and counts the frequency of each ascii character in it
// Returns a node array corresponding to the frequencies
NodeArray * createHistogramFromFile(char * input_file, int * ascii_freq){	
	FILE *fd;
	if ((fd = fopen(input_file, "r")) == NULL) {
		return 0;
	}
	int c;
	int counter = 0;
	while ((c=fgetc(fd)) != EOF) {
		ascii_freq[c]++;
		counter++;
	}
	NodeArray *nArray = initializeNodeArray(ascii_freq, counter);
	return nArray;

	// ===========================TEST==================================
	// Same as initializeNodeArray

	// NodeArray *nArray = (NodeArray *) malloc(counter * sizeof(NodeArray));
	// nArray->nChars = 0;
	// nArray->capacity = counter;
	// nArray->nodes = (Node**) malloc(nArray->capacity * sizeof(Node)); 
	
	// for (int i = 0; i < 256; i++) {
	// 	if (ascii_freq[i] != 0) {
	// 		insertInNodeArray(i, ascii_freq[i], nArray);
	// 	}
	// }

	// sortNodeArray(nArray);

	// =================================================================
}


// Prints the node array in the format "[character], [frequency]\n"
void printNodeArray(NodeArray * node_array){
	for (int i=0; i < node_array->nChars; i++){
		// Node * n = (Node*)((char*)node_array->nodes + i*sizeof(Node));
		// printf("%c, %u\n", n->val, n->frequency);
		
		// [Problem] Not sure why the original is not working
		printf("%c, %u\n", node_array->nodes[i]->val, node_array->nodes[i]->frequency);
	}
}

// Returns the minimum node of the node_array,
// assuming that the array has been sorted
// If the array is empty, returns NULL
Node * getMin(NodeArray * node_array){
	if (node_array->nChars > 0) 
		return (Node*)node_array->nodes;
	else return NULL;
}

//Selects the minimum of two nodes as the next child based on frequency
//Character value is used as a tie breaker if needed
Node* selectChild(Node* n1, Node* n2){
	if (compNodes(n1, n2) > 0) {
		return n2;
	} else {
		return n1;
	}
}

//Initializes a parent node by assigning its children
//Also sets its frequency to the sum of its children's frequencies
//and sets its value to the minimum value of its children
void initParent(Node* parent, Node* left, Node* right){
	parent->frequency = left->frequency + right->frequency;
	parent->left = left;
	parent->right = right;
	if(right->val < left->val) parent->val = right->val;
	else parent->val = left->val;
}


// Builds a Huffman Tree and associates characters to their code using a node_array
// Assumes that the node array has already been created and sorted
NodeArray * buildHuffmanTree(NodeArray * node_array){
	NodeArray * huffman_tree = (NodeArray *) malloc(512 * sizeof(NodeArray));
	huffman_tree->capacity = 512;
	huffman_tree->nodes = (Node**) malloc(huffman_tree->capacity * sizeof(Node));
	//Declare indicies for both arrays
	int leaf_index=0, internal_index=0; 
	//Declare the next available index to use for internal nodes
	int next_internal=0;

	// iterate through node_array and any created internal nodes to build the huffman tree:
	// (1) While any leaves or more than 1 non-leaves have not been turned into children, 
	// 		select the minimum two nodes from either node array
	// (2) Use these nodes as the children left and right passed to initParent
	// (3) Be sure to manage your indicies appropriately!
	Node *parent = (Node *) malloc(sizeof(Node));
	initParent(parent, node_array->nodes[leaf_index], node_array->nodes[leaf_index + 1]);
	leaf_index += 2;
	huffman_tree->nodes[next_internal] = parent;
	huffman_tree->nChars++;
	next_internal++;

	while (internal_index < next_internal && leaf_index < node_array->nChars) {
		Node *parent = (Node *) malloc(sizeof(Node));
		Node *internal = huffman_tree->nodes[internal_index];
		Node *internal2 = huffman_tree->nodes[internal_index + 1];
		Node *leaf = node_array->nodes[leaf_index];
		Node *leaf2 = node_array->nodes[leaf_index + 1];
		
		if (selectChild(leaf, internal)->val == leaf->val && leaf_index + 1 < node_array->nChars &&
			selectChild(leaf2, internal)->val == leaf2->val) {
			// Checking if the leaf node and the next consecutive leaf node are both smaller than the current internal node
			// If true then creating a node using 2 leaf nodes
			initParent(parent, node_array->nodes[leaf_index], node_array->nodes[leaf_index + 1]);
			parent->left = selectChild(node_array->nodes[leaf_index], node_array->nodes[leaf_index + 1]);
			if (parent->left->val == node_array->nodes[leaf_index]->val) {
				parent->right = node_array->nodes[leaf_index + 1];
			} else {
				parent->right = node_array->nodes[leaf_index];
			}
			leaf_index += 2;
		} else if (selectChild(leaf, internal)->val == internal->val 
					&& huffman_tree->nChars > 1 
					&& selectChild(leaf, internal2)->val == internal2->val) {
			// Creating an internal node from 2 consecutive internal nodes if both are smaller than the current leaf node
			initParent(parent, huffman_tree->nodes[internal_index], huffman_tree->nodes[internal_index + 1]);
			parent->left = selectChild(huffman_tree->nodes[internal_index], huffman_tree->nodes[internal_index + 1]);
			if (parent->left->val == huffman_tree->nodes[internal_index]->val) {
				parent->right = huffman_tree->nodes[internal_index + 1];
			}
			internal_index += 2;
		} else {
			// Creating an internal node from one internal and one leaf if previous statements fail
			initParent(parent, huffman_tree->nodes[internal_index], node_array->nodes[leaf_index]);
			parent->left = selectChild(huffman_tree->nodes[internal_index], node_array->nodes[leaf_index]);
			if (parent->left->val == huffman_tree->nodes[internal_index]->val) {
				parent->right = node_array->nodes[leaf_index];
			} else {
				parent->right = huffman_tree->nodes[internal_index];
			}
			leaf_index++;
			internal_index++;
		}

		// Inserting the parent node into huffman_tree 
		huffman_tree->nodes[next_internal] = parent;
		huffman_tree->nChars++;
		next_internal++;
	}
	// Accounting for possible leftover internal nodes (combining two subtrees into one binary tree)
	while (internal_index < next_internal - 1) {
		Node *parent = (Node *) malloc(sizeof(Node));
		// printf("2\n");
		initParent(parent, huffman_tree->nodes[internal_index], huffman_tree->nodes[internal_index + 1]);
		parent->left = selectChild(huffman_tree->nodes[internal_index], huffman_tree->nodes[internal_index + 1]);
		if (parent->left->val == huffman_tree->nodes[internal_index]->val) {
			parent->right = huffman_tree->nodes[internal_index + 1];
		} else {
			parent->right = huffman_tree->nodes[internal_index];
		}
		internal_index += 2;
		huffman_tree->nodes[next_internal] = parent;
		huffman_tree->nChars++;
		next_internal++;
	}
	// Assign the codes for the leaves (characters) of the Huffman Tree and then return it
	codifyChars(huffman_tree);
	return huffman_tree;
}


//Returns the root node of the Huffman Tree
Node * getRootHuffmanTree(NodeArray * huffman_tree){
	// [Problem] This is not returning the correct root
	// return (Node*) ((char*)huffman_tree->nodes + (huffman_tree->nChars-1)*sizeof(Node));
	return huffman_tree->nodes[huffman_tree->nChars - 1];
}

// Checks if a node is a leaf node or not
// A leaf node has no children (so its left and right pointers are NULL)
//
// Returns 1 if the node is a leaf, 0 otherwise
int isLeafNode(Node * n){
	if (n->left == NULL && n->right == NULL) return 1;
	else return 0;
}

// Prints a huffman tree recursively from the root
// Prints a node as "[ascii value] = [ascii character]\t[bitstring]]\n"
// Note: The ascii value should be padded with 0's to be 3 digits
// Note2: The left subtree is assigned a 1 bit, 
// 		and the right subtree is assigned a 0 bit
void printTree(Node * root, int code[], int i){
	if (root == NULL) return;
	// pre-order traversal
	if (isLeafNode(root)) {
		// printing a node as "[ascii value] = [ascii character]\t[bitstring]]\n"
		printf("%03d = %c\t%s\n", (int) root->val, root->val, char_to_code[root->val]);
	}
	// calling left subtree recursively
	if (root->left) printTree(root->left, code, i + 1);
	// calling right subtree recursively
	if (root->right) printTree(root->right, code, i + 1);
}	

// Traverses a Huffman Tree from the root to each node, and records each path
// As the code string for each character
//
// The left child of a node should have a 1 bit appended to their code
// The right child of a node should have a 0 bit appended to their code
void textToCode(Node * root, int code[], int i){
	if (root == NULL) return ;
	
	//Assign codes for the left subtree
	code[i] = 1;
	textToCode(root->left, code, i+1);

	//Assign codes for the right subtree
	code[i] = 0;
	textToCode(root->right, code, i+1);

	//Note: only leaves need to have a code assigned!

	if (isLeafNode(root)) {
		char_to_code[root->val] = (char *) malloc(i * sizeof(char));
		// dereferencing the pointer
		char *temp = char_to_code[root->val];
		for (int j = 0; j < i; j++) {
			// Converting int to char and shifting it by 48 to get the ascii value
			*temp = (char) code[j] + '0';
			temp++;
		}
		*temp = '\0';
	}
}

// Prints a huffman tree by calling printTree on the root node of the tree
void printHuffmanTree(NodeArray * huffman_tree){
	Node * root = getRootHuffmanTree(huffman_tree);

	int code[MAXCODELENGTH] = {0};
	printTree(root, code, 0);
}

// Calls textToCode on the root of the provided huffman tree
void codifyChars(NodeArray * huffman_tree){
	Node * root = getRootHuffmanTree(huffman_tree);

	int code[MAXCODELENGTH] = {0};
	textToCode(root, code, 0);	
}
