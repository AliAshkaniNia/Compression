#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "algorithms/LZWCompression.h"
#include "utility/integerToStringSerializer.h"

using namespace Algorithms;
using namespace Serializers;

class LZWCompressionTest : public ::testing::Test {
protected:
    // The setup code here will be run before each test
    void SetUp() override {
        // Create a serializer for use with the LZWCompression class
        serializer = std::make_unique<integerToStringSerializer<uint32_t>>(true);
        lzw = std::make_unique<LZWCompression>(std::move(serializer));
    }

    std::unique_ptr<LZWCompression> lzw;
    std::unique_ptr<IStringSerializer<uint32_t>> serializer;
};

TEST_F(LZWCompressionTest, TestEncodeDecode) {
    std::string input = "If you only do what you can do, you will never be more than you are now.";
    std::string encoded;
    std::string decoded;

    // Test encoding
    EXPECT_EQ(lzw->encode(input, encoded), 0);
    EXPECT_NE(encoded, input);

    // Test decoding
    EXPECT_EQ(lzw->decode(encoded, decoded), 0);
    EXPECT_EQ(decoded, input);
}

TEST_F(LZWCompressionTest, TestEncodeDecodeEmptyString) {
    std::string input = "";
    std::string encoded;
    std::string decoded;

    // Test encoding
    EXPECT_EQ(lzw->encode(input, encoded), 0);
    EXPECT_EQ(encoded, "");

    // Test decoding
    EXPECT_EQ(lzw->decode(encoded, decoded), 0);
    EXPECT_EQ(decoded, "");
}

TEST_F(LZWCompressionTest, TestEncodeDecodeRepeatedCharacter) {
    std::string input(100, 'a'); // string of 100 'a's
    std::string encoded;
    std::string decoded;

    // Test encoding
    EXPECT_EQ(lzw->encode(input, encoded), 0);
    EXPECT_NE(encoded, input);

    // Test decoding
    EXPECT_EQ(lzw->decode(encoded, decoded), 0);
    EXPECT_EQ(decoded, input);
}

TEST_F(LZWCompressionTest, TestEncodeSimpleText) {
    std::string input{"abc"}; // string of 100 'a's
    std::string encoded;
    std::string decoded;

    // Test encoding
    EXPECT_EQ(lzw->encode(input, encoded), 0);
    EXPECT_EQ(encoded, "000000610000006200000063");

    // Test decoding
    EXPECT_EQ(lzw->decode(encoded, decoded), 0);
    EXPECT_EQ(decoded, input);
}

TEST_F(LZWCompressionTest, TestIllFormedDecode) {
    std::string input(6, 'a'); // string of 100 'a's
    std::string decoded;
    
    EXPECT_EQ(lzw->decode(input, decoded), 1);
    EXPECT_EQ(decoded, "");
}