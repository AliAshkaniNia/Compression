#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "utility/integerToStringSerializer.h"
#include <limits>

using namespace Serializers;

TEST(IntegerToStringSerializerTest, TestUint32Serialization) {
    integerToStringSerializer<uint32_t> serializer(true);
    EXPECT_EQ(serializer.serialize(305419896), "12345678");
    EXPECT_EQ(serializer.deserialize("12345678"), 305419896);
}

TEST(IntegerToStringSerializerTest, TestUint32NonHumanSerialization) {
    integerToStringSerializer<uint32_t> serializer(false);
    std::string serialized = serializer.serialize(305419896);
    ASSERT_EQ(serialized.size(), sizeof(uint32_t));
    EXPECT_EQ(serializer.deserialize(serialized), 305419896);
}

TEST(IntegerToStringSerializerTest, TestIntSerialization) {
    integerToStringSerializer<int> serializer(true);
    EXPECT_EQ(serializer.serialize(-1), "ffffffff");
    EXPECT_EQ(serializer.deserialize("ffffffff"), -1);
}

TEST(IntegerToStringSerializerTest, TestIntNonHumanSerialization) {
    integerToStringSerializer<int> serializer(false);
    std::string serialized = serializer.serialize(-1);
    ASSERT_EQ(serialized.size(), sizeof(int));
    EXPECT_EQ(serializer.deserialize(serialized), -1);
}

TEST(IntegerToStringSerializerTest, TestUint8Serialization) {
    integerToStringSerializer<uint8_t> serializer(true);
    EXPECT_EQ(serializer.serialize(255), "ff");
    EXPECT_EQ(serializer.deserialize("ff"), 255);
}

TEST(IntegerToStringSerializerTest, TestUint8NonHumanSerialization) {
    integerToStringSerializer<uint8_t> serializer(false);
    std::string serialized = serializer.serialize(255);
    ASSERT_EQ(serialized.size(), sizeof(uint8_t));
    EXPECT_EQ(serializer.deserialize(serialized), 255);
}

TEST(IntegerToStringSerializerTest, TestDeserializeInvalidLength) {
    integerToStringSerializer<int> serializer(true);
    EXPECT_THROW(serializer.deserialize("1234567"), std::invalid_argument);
}

TEST(IntegerToStringSerializerTest, TestDeserializeInvalidLengthNonHuman) {
    integerToStringSerializer<int> serializer(false);
    EXPECT_THROW(serializer.deserialize("123"), std::invalid_argument);
}

