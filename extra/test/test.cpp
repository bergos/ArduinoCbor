#include <gtest/gtest.h>
#include <string.h>
#include <stdio.h>
#include "ArduinoCbor.h"

/* void fileDump(const char* filename, uint8_t* data, size_t size) {
  FILE* file = fopen(filename, "wb");
  fwrite(data, size, 1, file);
  fclose(file);
}

void arrayDump(uint8_t* data, size_t size) {
  printf("{");

  for(size_t i=0; i<size; i++) {
    printf("0x%02x, ", data[i]);
  }

  printf("}\n");
} */

uint8_t example[] = {
  0xa3, 0x66, 0x73, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x65, 0x76,
  0x61, 0x6c, 0x75, 0x65, 0x67, 0x69, 0x6e, 0x74, 0x65, 0x67,
  0x65, 0x72, 0x39, 0x04, 0xd1, 0x66, 0x6f, 0x62, 0x6a, 0x65,
  0x63, 0x74, 0xa1, 0x63, 0x6b, 0x65, 0x79, 0x65, 0x76, 0x61,
  0x6c, 0x75, 0x65
};

TEST(CborArray, items) {
  CborBuffer buffer(200);
  CborArray array = CborArray(buffer);

  array.add("value");
  array.add(-1234);

  ASSERT_STREQ(array.get(0).asString(), "value");
  ASSERT_EQ(array.get(1).asInteger(), -1234);
}

TEST(CborObject, properties) {
  CborBuffer buffer(200);
  CborObject object = CborObject(buffer);

  object.set("string", "value");
  object.set("integer", -1234);
  
  CborObject child = CborObject(buffer);
  child.set("key", "value");
  object.set("object", child);
  
  CborArray array = CborArray(buffer);
  array.add(-1234);
  object.set("array", array);

  ASSERT_TRUE(object.get("string").isString());
  ASSERT_FALSE(object.get("string").isInteger());
  ASSERT_FALSE(object.get("string").isObject());
  
  ASSERT_FALSE(object.get("integer").isString());
  ASSERT_TRUE(object.get("integer").isInteger());
  ASSERT_FALSE(object.get("integer").isObject());
  
  ASSERT_FALSE(object.get("object").isString());
  ASSERT_FALSE(object.get("object").isInteger());
  ASSERT_TRUE(object.get("object").isObject());

  ASSERT_STREQ(object.get("string").asString(), "value");
  ASSERT_EQ(object.get("integer").asInteger(), -1234);
  ASSERT_STREQ(object.get("object").asObject().get("key").asString(), "value");
}

TEST(CborObject, encode) {
  CborBuffer buffer(200);
  CborObject object = CborObject(buffer);

  object.set("string", "value");
  object.set("integer", -1234);
  
  CborObject child = CborObject(buffer);
  child.set("key", "value");
  object.set("object", child);

  uint8_t data[1024];

  size_t size = object.encode(data, sizeof(data));

  ASSERT_EQ(size, sizeof(example));
  ASSERT_EQ(memcmp(data, example, sizeof(example)), 0);
}

TEST(CborBuffer, decode) {
  CborBuffer buffer(200);
  CborVariant variant = buffer.decode(example, sizeof(example));
  CborObject object = variant.asObject();

  ASSERT_STREQ(object.get("string").asString(), "value");
  ASSERT_EQ(object.get("integer").asInteger(), -1234);
  ASSERT_STREQ(object.get("object").asObject().get("key").asString(), "value");
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
