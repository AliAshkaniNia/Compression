#ifndef __COMPRESSION_ALGORITHM_H__
#define __COMPRESSION_ALGORITHM_H__

#include <string>

class CompressionAlgorithm {
public:
    virtual void encode(const std::string& input, std::string& output) = 0;
    virtual void decode(const std::string& input, std::string& output) = 0;
    
    int saveToFile(const std::string& filename, const std::string& data);
    int loadFromFile(const std::string& filename, std::string& data);

    virtual ~CompressionAlgorithm() = default;
};

#endif 