<h1 align="center">huffman-coding</h1>

## Description:
Lossless data compression algorithm. Uses binary tree structure to organize the characters by their frequency.

<p align="center">
  <a href="https://en.wikipedia.org/wiki/Huffman_coding"><img src="./images/huffman.png"/></a>
</p>

## Usage:
1) Download the files in the repo
2) Navigate to the folder containing the files
3) Compile huffman file using the make file
```
make
```
4) Flags
* Type "./huffman [mode] [arguments]" where mode and its arguments are one of the following:
* -p [src]: print the huffman tree of the file
* -pc [src]: print the huffman tree of an encoded file (using frequencies listed in its header)
* -pn [src]: print the node array of the file
* -e [src] [dest]: encode the source file and place the result into the specified destination
* -ep [src] [dest]: encode and print the resulting huffman tree
* -d [src] [dest]: decode the source file and place the result into the specified destination
* -dpc [src] [dest]: decode and print the resulting huffman tree
For example:
```
./huffman -e hello.txt encoded.huff
```
This will encode a file hello.txt and save it to the file econded.huff.
