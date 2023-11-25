

#ifndef __INTEGER_TO_STRING_SERIALIZER_H__
#define __INTEGER_TO_STRING_SERIALIZER_H__

#include "iStringSerializer.h"
#include <string_view>
#include <iomanip>

/**
 * @brief serializes and deserializes integral values to and from string representations.
 *
 * The integerToStringSerializer class provides functionality to serialize a n-bit
 * integer into a string representation. It also allows decoding the string back to the
 * original integer value. The encoding can be either human-readable or non-human-readable,
 * depending on the specified option.
 *
 * @note If human_readable is true, the function returns a human-readable hex representation
 *       of the input value, e.g., "12345678". If human_readable is false, the function returns
 *       a non-human-readable representation of the input value, e.g., "x4Vh".
 *
 * @example
 *  uint32_t num = 305419896; // 305419896 in decimal is 0x12345678 in hex
 *  Human-readable string encoding: 12345678
 *  Non-human-readable string encoding: x4Vh
 *
 * integerToStringSerializer's implementation is in this header due to its template nature.
 * This allows the compiler to access the full template definition at compile time.
 * As templates are compiled at the place they are instantiated, not where they are defined.
 *
 */

namespace Serializers
{
    template <typename T>
    class integerToStringSerializer : public IStringSerializer<T>
    {
    public:
        integerToStringSerializer(bool human_readable) : human_readable(human_readable)
        {
            serialized_word_size = human_readable? sizeof(T)*2: sizeof(T);
        }

        /**
         * If it's challenging for you to follow what's going on in this function,
         * It is just a generalized version of the code below
         *     str[0] = (static_cast<char>((num >> 24) & 0xFF));
         *     str[1] = (static_cast<char>((num >> 16) & 0xFF));
         *     str[2] = (static_cast<char>((num >> 8) & 0xFF));
         *     str[3] = (static_cast<char>(num & 0xFF));
         *
         */
        std::string serialize(const T &num) override
        {
            static_assert(std::is_integral<T>::value, "Integral type required");

            if (human_readable)
            {
                std::stringstream ss;
                for (int i = sizeof(T) - 1; i >= 0; --i)
                {
                    ss << std::hex << std::setfill('0') << std::setw(2)
                       << static_cast<int>((num >> (i * 8)) & 0xff);
                }
                return ss.str();
            }

            std::string serializedString(sizeof(T), '=');
            for (std::size_t i = 0; i < sizeof(T); ++i)
            {
                serializedString[sizeof(T) - i - 1] = static_cast<char>((num >> (i * 8)) & 0xFF);
            }
            return serializedString;
        }

        T deserialize(std::string_view serializedString) override
        {
            static_assert(std::is_integral<T>::value, "Integral type required");
    
            if (human_readable)
            {
                if (serializedString.size() != serialized_word_size)
                {
                    std::cerr<<"Error in deserializeing, The string "<< serializedString <<" should contain exactly " << serialized_word_size <<" bytes\n";
                    std::cerr<<"Try turning the human-readable option off \n";
                    throw std::invalid_argument("");

                    // Handle error: The string should contain exactly sizeof(T) bytes.
                    // return 0;
                }

                T result = 0;
                for (std::size_t i = 0; i < sizeof(T); ++i)
                {
                    std::stringstream ss;
                    ss << std::hex << serializedString.substr(i * 2, 2);
                    int byte;
                    ss >> byte;
                    result |= static_cast<T>(byte) << ((sizeof(T) - 1 - i) * 8);
                }
                return result;
            }
            
            if (serializedString.size() != serialized_word_size)
            {
                std::cerr<<"Error in deserializeing, The string "<< serializedString <<" should contain exactly " << serialized_word_size <<" bytes\n";
                std::cerr<<"Try turning the human-readable option on \n";
                throw std::invalid_argument("");
            }

            T result = 0;
            for (std::size_t i = 0; i < sizeof(T); ++i)
            {
                result |= static_cast<T>(static_cast<unsigned char>(serializedString[sizeof(T) - 1 - i])) << (i * 8);
            }
            return result;
        }
        size_t getSerializedWordSize() override{
            return serialized_word_size;
        }

    private:
        bool human_readable;
        size_t serialized_word_size;
    };

};

#endif