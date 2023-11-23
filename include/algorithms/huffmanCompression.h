#ifndef __HUFFMAN_COMPRESSION_H__
#define __HUFFMAN_COMPRESSION_H__

#include "compressionAlgorithm.h"
#include <unordered_map>
#include <queue>

class HuffmanCompression : public CompressionAlgorithm {
public:
    void encode(const std::string& input, std::string& output) override;
    void decode(const std::string& input, std::string& output) override;
    HuffmanCompression();

private:
    struct HuffmanNode {
        char data;
        int frequency;
        HuffmanNode* left;
        HuffmanNode* right;

        HuffmanNode(char data, unsigned frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
        HuffmanNode(char data, unsigned frequency, 
            HuffmanNode* left, HuffmanNode* right) : 
            data(data), frequency(frequency), left(left), right(right) {}

        bool operator>(const HuffmanNode& other) const {
            return frequency > other.frequency;
        }
    };

    void buildHuffmanTree(const std::string& input);
    void buildHuffmanCodes(HuffmanNode* root, const std::string& code);
    void encodeTree(HuffmanNode* root, std::string& encodedTree);
    void decodeTree(const std::string& encodedTree, size_t& index, HuffmanNode*& root);

    void addNode(HuffmanNode* root, char ch, const std::string &code);

    HuffmanNode* huffmanTreeRoot;
    std::unordered_map<char, std::string> huffmanCodes;
};


#endif