#include "huffmanCompression.h"
#include <vector>
#include <functional>
#include <sstream>
#include <iostream>

HuffmanCompression::HuffmanCompression() : huffmanTreeRoot(nullptr) {}

void HuffmanCompression::buildHuffmanTree(const std::string& input) {
    // Count the frequency of each character in the input string
    std::unordered_map<char, unsigned> frequencyMap;
    for (char c : input) {
        frequencyMap[c]++;
    }

    // Build the priority queue of Huffman nodes
    std::priority_queue<HuffmanNode, std::vector<HuffmanNode>, std::greater<>> pq;
    for (const auto& pair : frequencyMap) {
        pq.push(HuffmanNode(pair.first, pair.second));
    }

    // Build the Huffman tree
    while (pq.size() > 1) {
        HuffmanNode* left = new HuffmanNode(pq.top());
        pq.pop();
        HuffmanNode* right = new HuffmanNode(pq.top());
        pq.pop();

        HuffmanNode* combinedNode = new HuffmanNode('\0', left->frequency + right->frequency);
        combinedNode->left = left;
        combinedNode->right = right;

        pq.push(*combinedNode);
    }

    huffmanTreeRoot = new HuffmanNode(pq.top());
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

void HuffmanCompression::encode(const std::string& input, std::string& output) {
    buildHuffmanTree(input);
    buildHuffmanCodes(huffmanTreeRoot, "");
    
    std::stringstream ss;

    for (const auto& entry : huffmanCodes) {
        ss << entry.first << entry.second << ' ';
    }
    ss << '\n';

    for (char ch : input) {
        ss << huffmanCodes[ch];
    }
    ss << '\n';

    output = ss.str();
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

void HuffmanCompression::decode(const std::string& input, std::string& output) {

    // finding tree 
    output="";

    size_t pos = input.find('\n');
    if (pos == std::string::npos) {
        std::cerr << "Ill-formed input for decoding (does not contain a tree)" << std::endl;
        return ; 
    } 
    
    std::string huffman_tree = input.substr(0, pos);
    std::string encoded_string = input.substr(pos+1);
    encoded_string.pop_back();

    char character;
    std::string code;
    std::istringstream iss(huffman_tree);
    huffmanTreeRoot = new HuffmanNode('\0', 0);

    while (iss >> character >> code) {
        addNode(huffmanTreeRoot, character, code);
    }

    HuffmanNode* currentNode = huffmanTreeRoot;

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

}