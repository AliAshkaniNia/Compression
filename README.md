# File Compression Utility 

This project is a simple file compression utility, currently it supports huffman and LZW Algorithms. 

## Desing 
UML graph 

### Huffman algorithm 
https://www.youtube.com/watch?v=JsTptu56GM8

Serialization : 
I used a pretty simple and human readable format : 

4 byte (tree len)+ tree + encoded data 

### LZW 
https://www.youtube.com/watch?v=1KzUikIae6k

## Usage 


It also works with pipes. 


## Dependencies 
This library uses [cxxopts](https://github.com/jarro2783/cxxopts) to parse command line arguments. 


C++17 compiler should work fine. (string_view)