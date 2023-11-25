#ifndef __LZW_COMPRESSION_H__
#define __LZW_COMPRESSION_H__

#include "iAlgorithm.h"
#include <memory>
#include "utility/iStringSerializer.h"

namespace Algorithms
{
    class LZWCompression : public IAlgorithm
    {
    public:
        int encode(const std::string &input, std::string &output) override;
        int decode(const std::string &input, std::string &output) override;
        LZWCompression() = delete;
        LZWCompression(std::unique_ptr<Serializers::IStringSerializer<uint32_t>> serializer);

    private:
        std::unique_ptr<Serializers::IStringSerializer<uint32_t>> m_serializer;
    };
};

#endif