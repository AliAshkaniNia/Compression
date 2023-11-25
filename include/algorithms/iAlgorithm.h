#ifndef __I_ALGORITHM_H__
#define __I_ALGORITHM_H__

#include <string>
namespace Algorithms
{
    class IAlgorithm
    {
    public:
        virtual int encode(const std::string &input, std::string &output) = 0;
        virtual int decode(const std::string &input, std::string &output) = 0;

        virtual ~IAlgorithm() = default;
    };
};
#endif