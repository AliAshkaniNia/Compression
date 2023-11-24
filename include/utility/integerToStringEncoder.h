
#include "iStringEncoder.h"
#include <string_view>
#include <iomanip>

#ifndef __INTEGER_TO_STRING_ENCODER_H__
#define __INTEGER_TO_STRING_ENCODER_H__

/**
 * @brief Encodes and decodes integral values to and from string representations.
 *
 * The IntegerToStringEncoder class provides functionality to encode a n-bit
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
 * IntegerToStringEncoder's implementation is in this header due to its template nature.
 * This allows the compiler to access the full template definition at compile time.
 * As templates are compiled at the place they are instantiated, not where they are defined.
 *
 */

namespace Converters
{
    template <typename T>
    class IntegerToStringEncoder : public IStringEncoder<T>
    {
    public:
        IntegerToStringEncoder(bool human_readable) : human_readable(human_readable)
        {
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
        std::string encode(const T &num) override
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

            std::string encodedString(sizeof(T), '=');
            for (std::size_t i = 0; i < sizeof(T); ++i)
            {
                encodedString[sizeof(T) - i - 1] = static_cast<char>((num >> (i * 8)) & 0xFF);
            }
            return encodedString;
        }

        T decode(std::string_view encodedString) override
        {
            static_assert(std::is_integral<T>::value, "Integral type required");

            if (encodedString.size() != sizeof(T))
            {
                // Handle error: The string should contain exactly sizeof(T) bytes.
                return 0;
            }

            if (human_readable)
            {
                T result = 0;
                for (std::size_t i = 0; i < sizeof(T); ++i)
                {
                    std::stringstream ss;
                    ss << std::hex << encodedString.substr(i * 2, 2);
                    int byte;
                    ss >> byte;
                    result |= static_cast<T>(byte) << ((sizeof(T) - 1 - i) * 8);
                }
                return result;
            }

            T result = 0;
            for (std::size_t i = 0; i < sizeof(T); ++i)
            {
                result |= static_cast<T>(static_cast<unsigned char>(encodedString[sizeof(T) - 1 - i])) << (i * 8);
            }
            return result;
        }

        bool is_human_readable() const
        {
            return human_readable;
        }

    private:
        bool human_readable;
    };

};

#endif