#ifndef __I_ALGORITHM_H__
#define __I_ALGORITHM_H__

#include <string>
#include <string_view>

namespace Algorithms
{
    class IAlgorithm
    {
    public:
        virtual int encode(std::string_view input, std::string &output) = 0;
        virtual int decode(std::string_view input, std::string &output) = 0;

        virtual ~IAlgorithm() = default;
    };
};
#endif