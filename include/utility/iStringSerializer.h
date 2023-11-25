#include <string>

#ifndef __I_STRING_SERIALZER_H__
#define __I_STRING_SERIALZER_H__

namespace Serializers
{
    template <typename T>
    class IStringSerializer
    {
    public:
        virtual ~IStringSerializer() = default;

        virtual std::string serialize(const T &num) = 0;

        virtual T deserialize(std::string_view serializedString) = 0;
    };
};

#endif