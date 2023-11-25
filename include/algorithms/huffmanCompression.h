#ifndef __HUFFMAN_COMPRESSION_H__
#define __HUFFMAN_COMPRESSION_H__

#include "iAlgorithm.h"
#include <unordered_map>
#include <queue>
#include <memory>
#include <string_view>
#include "utility/iStringSerializer.h"
namespace Algorithms
{

    class HuffmanCompression : public IAlgorithm
    {
    public:
        int encode(std::string_view input, std::string &output) override;
        int decode(std::string_view input, std::string &output) override;
        HuffmanCompression() = delete;
        HuffmanCompression(std::unique_ptr<Serializers::IStringSerializer<uint32_t>> serializer);
        ~HuffmanCompression();
    private:
        struct HuffmanTreeNode
        {
            char data;
            int frequency;
            ///TODO: Use smart pointers instead of these raw ones
            HuffmanTreeNode *left;
            HuffmanTreeNode *right;

            HuffmanTreeNode(char data, unsigned frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
            HuffmanTreeNode(char data, unsigned frequency,
                        HuffmanTreeNode *left, HuffmanTreeNode *right) : data(data), frequency(frequency), left(left), right(right) {}

            bool operator<(const HuffmanTreeNode &other) const
            {
                // Notice the '>' here. 
                // This is because priority_queue uses this operator to order 
                // the elements in reverse, creating a min heap.

                return frequency > other.frequency; 
            }
        };
        void createTree(std::string_view input);
        void createCodes(HuffmanTreeNode *root, const std::string &code);

        void deleteTree(HuffmanTreeNode* root);

        void encodeTree(HuffmanTreeNode *root, std::string &encodedTree);
        void parseTree(std::string_view encodedTree);

        void addTreeNode(HuffmanTreeNode *root, char ch, std::string_view code);

        HuffmanTreeNode *huffmanTreeRoot;
        std::unordered_map<char, std::string> huffmanCodes;

        std::unique_ptr<Serializers::IStringSerializer<uint32_t>> m_serializer;
    };

};
#endif