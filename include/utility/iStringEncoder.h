#include <string>

#ifndef __I_COVERTER_H__
#define __I_COVERTER_H__

namespace Converters
{
    template <typename T>
    class IStringEncoder
    {
    public:
        virtual ~IStringEncoder() = default;

        virtual std::string encode(const T &num) = 0;

        virtual T decode(std::string_view encodedString) = 0;
    };
};

#endif