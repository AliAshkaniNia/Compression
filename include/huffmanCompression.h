#ifndef __HUFFMAN_COMPRESSION_H__
#define __HUFFMAN_COMPRESSION_H__

#include "compressionAlgorithm.h"

class HuffmanCompression : public CompressionAlgorithm {
public:
    void encode(const std::string& input, std::string& output) override;
    void decode(const std::string& input, std::string& output) override;
    HuffmanCompression() = default;
};


#endif