#ifndef __LZW_COMPRESSION_H__
#define __LZW_COMPRESSION_H__

#include "compressionAlgorithm.h"
///TODO: USE NAMESPACE 
class LZWCompression : public CompressionAlgorithm {
public:
    void encode(const std::string& input, std::string& output) override;
    void decode(const std::string& input, std::string& output) override;
    LZWCompression() = default;

};


#endif