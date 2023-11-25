#ifndef __LZW_COMPRESSION_H__
#define __LZW_COMPRESSION_H__

#include "compressionAlgorithm.h"
#include <memory>
#include "utility/iStringSerializer.h"

///TODO: USE NAMESPACE 
class LZWCompression : public CompressionAlgorithm {
public:
    void encode(const std::string& input, std::string& output) override;
    void decode(const std::string& input, std::string& output) override;
    LZWCompression() = delete;
    LZWCompression(std::unique_ptr<Serializers::IStringSerializer<uint32_t>> serializer);

private:
    std::unique_ptr<Serializers::IStringSerializer<uint32_t>> m_serializer;

};


#endif