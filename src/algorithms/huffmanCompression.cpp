#include "algorithms/huffmanCompression.h"

#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>


Algorithms::HuffmanCompression::HuffmanCompression(std::unique_ptr<Serializers::IStringSerializer<uint32_t>> serializer)
    :huffmanTreeRoot(nullptr),
    m_serializer(std::move(serializer)){

}

void Algorithms::HuffmanCompression::createTree(std::string_view  input) {
    // Count the frequency of each character in the input string
    std::unordered_map<char, unsigned> frequencyMap;
    for (char c : input) {
        frequencyMap[c]++;
    }

    // Build the priority queue of Huffman nodes
    std::priority_queue<HuffmanTreeNode*> pq;
    for (const auto& pair : frequencyMap) {
        pq.push(new HuffmanTreeNode(pair.first, pair.second));
    }

    // Build the Huffman tree
    while (pq.size() > 1) {
        HuffmanTreeNode* left = pq.top();
        pq.pop();
        HuffmanTreeNode* right = pq.top();
        pq.pop();

        HuffmanTreeNode* combinedNode = new HuffmanTreeNode('\0', left->frequency + right->frequency, left, right);

        pq.push(combinedNode);
    }
    // Deleting  previous tree to prevent memory leak
    if(huffmanTreeRoot != nullptr){
        deleteTree(huffmanTreeRoot);
    }
    huffmanTreeRoot = pq.top();
    pq.pop();
}

void Algorithms::HuffmanCompression::createCodes(HuffmanTreeNode* root, const std::string& code) {
    if (!root) {
        return;
    }
    if (!root->left && !root->right) {
        huffmanCodes[root->data] = code;
    }
    createCodes(root->left, code + '0');
    createCodes(root->right, code + '1');
}

int Algorithms::HuffmanCompression::encode(std::string_view input, std::string& output) {
    createTree(input);
    createCodes(huffmanTreeRoot, "");
    
    std::stringstream encodedStream, treeStream;

    for (const auto& entry : huffmanCodes) {
        treeStream << entry.first << entry.second << ' ';
    }

    encodedStream << m_serializer->serialize(treeStream.str().length()) << "\n";
    encodedStream << treeStream.str() << "\n";

    for (const auto& ch : input) {
        encodedStream << huffmanCodes[ch];
    }
    // a trailing new line just to look nice
    encodedStream << '\n';

    output = encodedStream.str();
    return 0;
}

void Algorithms::HuffmanCompression::addTreeNode(HuffmanTreeNode* root, char ch, std::string_view code) {
    HuffmanTreeNode* node = root;
    for(auto &c : code) {
        if(c == '0') {
            if (!node->left) {
                node->left = new HuffmanTreeNode('\0', 0);
            }
            node = node->left;
        } else {
            if (!node->right) {
                node->right = new HuffmanTreeNode('\0', 0);
            }
            node = node->right;
        }
    }
    node->data = ch;
}
void Algorithms::HuffmanCompression::parseTree(std::string_view encodedTree) {

    std::string huffmanCode;

    // Deleting  previous tree to prevent memory leak
    if(huffmanTreeRoot != nullptr){
        deleteTree(huffmanTreeRoot);
    }
    huffmanTreeRoot = new HuffmanTreeNode('\0', 0);
  
    size_t index = 0;
    while (index < encodedTree.size()) {
        char decodedChar = encodedTree[index];
        index++;
        
        huffmanCode = "";
        while (index < encodedTree.size() && encodedTree[index] != ' ') {
            huffmanCode += encodedTree[index];
            index++;
        }
        
        addTreeNode(huffmanTreeRoot, decodedChar, huffmanCode);

        index++; // skip over space
    }
}

int Algorithms::HuffmanCompression::decode(std::string_view  input, std::string& output) {

    // finding sections 
    output.clear();
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

    parseTree(huffman_tree);

    HuffmanTreeNode* currentNode = huffmanTreeRoot;
    
    // Deleting trailing new line
    encoded_string.pop_back();

    //decoding output
    for (char bit : encoded_string) {
        if (bit == '0') {
            if (currentNode && currentNode->left)
                currentNode = currentNode->left;
        } else {
            if (currentNode && currentNode->right) 
                currentNode = currentNode->right;
        }

        
        if ( currentNode && !currentNode->left && !currentNode->right) {
            output += currentNode->data;
            currentNode = huffmanTreeRoot;
        }
    }
    return 0;
}

void Algorithms::HuffmanCompression::deleteTree(HuffmanTreeNode* root){
    if(root==NULL) return;

    if(root->left) deleteTree(root->left);
    if(root->right) deleteTree(root->right);
    delete root;
}

Algorithms::HuffmanCompression::~HuffmanCompression(){
    deleteTree(huffmanTreeRoot);
}