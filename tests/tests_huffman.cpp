#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "algorithms/huffmanCompression.h"
#include "utility/integerToStringSerializer.h"

using namespace Algorithms;
using namespace Serializers;

class HuffmanCompressionTest : public ::testing::Test {
protected:
    // The setup code here will be run before each test
    void SetUp() override {
        // Create a serializer for use with the HuffmanCompression class
        serializer = std::make_unique<integerToStringSerializer<uint32_t>>(true);
        huffman = std::make_unique<HuffmanCompression>(std::move(serializer));
    }

    std::unique_ptr<HuffmanCompression> huffman;
    std::unique_ptr<IStringSerializer<uint32_t>> serializer;
};

TEST_F(HuffmanCompressionTest, TestEncodeDecode) {
    std::string input = "If comparable, it is no longer Bugatti.";
    std::string encoded;
    std::string decoded;

    // Test encoding
    EXPECT_EQ(huffman->encode(input, encoded), 0);
    EXPECT_NE(encoded, input);

    // Test decoding
    EXPECT_EQ(huffman->decode(encoded, decoded), 0);
    EXPECT_EQ(decoded, input);
}

TEST_F(HuffmanCompressionTest, TestEncodeDecodeEmptyString) {
    std::string input = "";
    std::string encoded;
    std::string decoded;

    // Test encoding
    EXPECT_EQ(huffman->encode(input, encoded), 0);
    EXPECT_EQ(encoded, "");

    // Test decoding
    EXPECT_EQ(huffman->decode(encoded, decoded), 0);
    EXPECT_EQ(decoded, "");
}

// This test case actually uncovered a bug!
TEST_F(HuffmanCompressionTest, TestEncodeDecodeRepeatedCharacter) {
    std::string input(100, 'a'); // string of 100 'a's
    std::string encoded;
    std::string decoded;

    // Test encoding
    EXPECT_EQ(huffman->encode(input, encoded), 0);
    EXPECT_NE(encoded, input);

    // Test decoding
    EXPECT_EQ(huffman->decode(encoded, decoded), 0);
    EXPECT_EQ(decoded, input);
}

TEST_F(HuffmanCompressionTest, TestIllFormedDecode) {
    std::string input(100, 'a'); // string of 100 'a's
    std::string decoded;
    
    EXPECT_EQ(huffman->decode(input, decoded), 1);
    EXPECT_EQ(decoded, "");
}