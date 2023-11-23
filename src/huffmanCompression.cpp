#include "huffmanCompression.h"
#include <vector>
#include <functional>
#include <sstream>
#include <iostream>
#include <stdint.h>
#include <iomanip>

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

// I used uint32_t instead of unsigned int for portability issues 
// human readbale format ex
/**
 * @brief Encodes a 32-bit unsigned integer into a string representation.
 *
 * This function encodes a 32-bit unsigned integer (value) into a string representation.
 * The encoding can be either human-readable or non-human-readable, depending on the value
 * of the human_readable parameter.
 *
 * @param value The unsigned integer value to be encoded.
 * @param human_readable Specifies whether the encoding should be human-readable or not.
 *        Default value is false.
 *
 * @return The encoded string representation of the input value.
 *
 * @note If human_readable is true, the function returns a human-readable hex representation
 *       of the input value, e.g., "12345678". If human_readable is false, the function returns
 *       a non-human-readable representation of the input value, e.g., "x4Vh".
 *
 * @example
 * uint32_t num = 305419896; // 305419896 in decimal is 0x12345678 in hex
 * std::string encoded = encode_uint32(num, true);
 * std::cout << "Human-readable encoding: " << encoded << std::endl;
 *
 * encoded = encode_uint32(num, false);
 * std::cout << "Non-human-readable encoding: " << encoded << std::endl;
 *
 * // Output:
 * // Human-readable encoding: 12345678
 * // Non-human-readable encoding: x4Vh
 */
std::string encode_uint32(uint32_t value, bool human_readable = false) {

    if(human_readable){
        std::stringstream ss;
        for (int i = 3; i >= 0; --i) {
            ss << std::hex << std::setfill('0') << std::setw(2)
            << ((value >> (i * 8)) & 0xff);
        }
        return ss.str();
    }
    
    std::string encodedString(4, '=');
    encodedString[0] = (static_cast<char>((value >> 24) & 0xFF));
    encodedString[1] = (static_cast<char>((value >> 16) & 0xFF));
    encodedString[2] = (static_cast<char>((value >> 8) & 0xFF));
    encodedString[3] = (static_cast<char>(value & 0xFF));
    return encodedString;
}

uint32_t decode_uint32(const std::string &encodedString,  bool human_readable = false) {
    if (encodedString.size() != 4) {
        // Handle error: The string should contain exactly 4 bytes.
        return 0;
    }

    if(human_readable){
        int result = 0;
        for (int i = 0; i < 4; ++i) {
            std::stringstream ss;
            ss << std::hex << encodedString.substr(i * 2, 2);
            int byte;
            ss >> byte;
            result |= byte << ((3 - i) * 8);
        }
        return result;
    }

    return (static_cast<uint32_t>(encodedString[0]) << 24) |
           (static_cast<uint32_t>(encodedString[1]) << 16) |
           (static_cast<uint32_t>(encodedString[2]) << 8) |
           static_cast<uint32_t>(encodedString[3]);
}

void HuffmanCompression::encode(const std::string& input, std::string& output) {
    buildHuffmanTree(input);
    buildHuffmanCodes(huffmanTreeRoot, "");
    
    std::stringstream ss,ss_tree;

    for (const auto& entry : huffmanCodes) {
        ss_tree << entry.first << entry.second << ' ';
    }

    ss<<encode_uint32(ss_tree.str().length()) << "\n";
    ss << ss_tree.str() << "\n";

    for (char ch : input) {
        ss << huffmanCodes[ch];
    }
    // a trailing new line just to look nice
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
    std::string tree_len_str = input.substr(0, 4);

    uint32_t tree_len = decode_uint32(tree_len_str);
    
    std::string huffman_tree = input.substr(4+1, tree_len);
    std::string encoded_string = input.substr(4+1+tree_len+1);

    encoded_string.pop_back();

    char character;
    std::string code;
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