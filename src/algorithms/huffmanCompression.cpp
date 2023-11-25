#include "algorithms/huffmanCompression.h"
#include <vector>
#include <functional>
#include <sstream>
#include <iostream>
#include <stdint.h>
#include <iomanip>

using namespace Algorithms;
HuffmanCompression::HuffmanCompression(std::unique_ptr<Serializers::IStringSerializer<uint32_t>> serializer)
    :m_serializer(std::move(serializer)), 
    huffmanTreeRoot(nullptr){

}

void HuffmanCompression::buildHuffmanTree(const std::string& input) {
    // Count the frequency of each character in the input string
    std::unordered_map<char, unsigned> frequencyMap;
    for (char c : input) {
        frequencyMap[c]++;
    }

    // Build the priority queue of Huffman nodes
    // std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, std::less<>> pq;
    std::priority_queue<HuffmanNode*> pq;
    for (const auto& pair : frequencyMap) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    // Build the Huffman tree
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top();
        pq.pop();
        HuffmanNode* right = pq.top();
        pq.pop();

        HuffmanNode* combinedNode = new HuffmanNode('\0', left->frequency + right->frequency, left, right);

        pq.push(combinedNode);
    }
    // Deleting  previous tree to prevent memory leak
    if(huffmanTreeRoot != nullptr){
        deleteTree(huffmanTreeRoot);
    }
    huffmanTreeRoot = pq.top();
    pq.pop();
}

void HuffmanCompression::buildHuffmanCodes(HuffmanNode* root, const std::string& code) {
    if (!root) {
        return;
    }
    if (!root->left && !root->right) {
        huffmanCodes[root->data] = code;
    }
    buildHuffmanCodes(root->left, code + '0');
    buildHuffmanCodes(root->right, code + '1');
}


int HuffmanCompression::encode(const std::string& input, std::string& output) {
    buildHuffmanTree(input);
    buildHuffmanCodes(huffmanTreeRoot, "");
    
    std::stringstream ss,ss_tree;

    for (const auto& entry : huffmanCodes) {
        ss_tree << entry.first << entry.second << ' ';
    }

    ss<<m_serializer->serialize(ss_tree.str().length()) << "\n";
    ss << ss_tree.str() << "\n";

    for (char ch : input) {
        ss << huffmanCodes[ch];
    }
    // a trailing new line just to look nice
    ss << '\n';

    output = ss.str();
    return 0;
}
// void addNode(HuffmanNode* root, char ch, const std::string &code);

void HuffmanCompression::addNode(HuffmanNode* root, char ch, const std::string &code) {
    HuffmanNode* node = root;
    for(auto &c : code) {
        if(c == '0') {
            if (!node->left) {
                node->left = new HuffmanNode('\0', 0);
            }
            node = node->left;
        } else {
            if (!node->right) {
                node->right = new HuffmanNode('\0', 0);
            }
            node = node->right;
        }
    }
    node->data = ch;
}

int HuffmanCompression::decode(const std::string& input, std::string& output) {

    // finding tree 
    output="";
    std::size_t serialized_word_size = m_serializer->getSerializedWordSize();
    std::string tree_len_str, huffman_tree, encoded_string;
    try{
        // The file begins with serialized_word_size bytes containing the tree length
        tree_len_str = input.substr(0, serialized_word_size);

        uint32_t tree_len = m_serializer->deserialize(tree_len_str); 
        
        //The tree length is followed by an additional '\n', so to reach the original tree, we should start from 'serialized_word_size+1'.
        huffman_tree = input.substr(serialized_word_size+1, tree_len);

        // Tree encoding is followed by an additional '\n', so to reach the encoded data, we need to start from 'serialized_word_size + 1 + tree_len + 1'.
        encoded_string = input.substr(serialized_word_size+1+tree_len+1);
    }catch(...){
        std::cerr<< "ill-formed input file for decoding\n";
        return 1;
    }
    // Deleting trailing new line
    encoded_string.pop_back();

    // Parsing tree 
    char character;
    std::string code;

    // Deleting  previous tree to prevent memory leak
    if(huffmanTreeRoot != nullptr){
        deleteTree(huffmanTreeRoot);
    }
    huffmanTreeRoot = new HuffmanNode('\0', 0);
  
    int i = 0;
    while (i < huffman_tree.size()) {
        character = huffman_tree[i];
        i++;
        
        code = "";
        while (i < huffman_tree.size() && huffman_tree[i] != ' ') {
            code += huffman_tree[i];
            i++;
        }
        
        addNode(huffmanTreeRoot, character, code);

        i++; // skip over space
    }

    HuffmanNode* currentNode = huffmanTreeRoot;
    //decoding output
    for (char bit : encoded_string) {
        if (bit == '0') {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }

        if (!currentNode->left && !currentNode->right) {
            output += currentNode->data;
            currentNode = huffmanTreeRoot;
        }
    }
    return 0;
}

void HuffmanCompression::deleteTree(HuffmanNode* root){
    if(root==NULL) return;

    if(root->left) deleteTree(root->left);
    if(root->right) deleteTree(root->right);
    delete root;
}

HuffmanCompression::~HuffmanCompression(){
    deleteTree(huffmanTreeRoot);
}