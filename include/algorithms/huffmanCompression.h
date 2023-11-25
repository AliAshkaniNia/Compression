#ifndef __HUFFMAN_COMPRESSION_H__
#define __HUFFMAN_COMPRESSION_H__

#include "iAlgorithm.h"
#include <unordered_map>
#include <queue>
#include <memory>
#include "utility/iStringSerializer.h"
namespace Algorithms
{

    class HuffmanCompression : public IAlgorithm
    {
    public:
        int encode(const std::string &input, std::string &output) override;
        int decode(const std::string &input, std::string &output) override;
        HuffmanCompression() = delete;
        HuffmanCompression(std::unique_ptr<Serializers::IStringSerializer<uint32_t>> serializer);

    private:
        struct HuffmanNode
        {
            char data;
            int frequency;
            HuffmanNode *left;
            HuffmanNode *right;

            HuffmanNode(char data, unsigned frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
            HuffmanNode(char data, unsigned frequency,
                        HuffmanNode *left, HuffmanNode *right) : data(data), frequency(frequency), left(left), right(right) {}

            bool operator>(const HuffmanNode &other) const
            {
                return frequency > other.frequency;
            }
        };

        void buildHuffmanTree(const std::string &input);
        void buildHuffmanCodes(HuffmanNode *root, const std::string &code);
        void encodeTree(HuffmanNode *root, std::string &encodedTree);
        void decodeTree(const std::string &encodedTree, size_t &index, HuffmanNode *&root);

        void addNode(HuffmanNode *root, char ch, const std::string &code);

        HuffmanNode *huffmanTreeRoot;
        std::unordered_map<char, std::string> huffmanCodes;

        std::unique_ptr<Serializers::IStringSerializer<uint32_t>> m_serializer;
    };

};
#endif