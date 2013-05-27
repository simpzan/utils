#include <iostream>
#include <gtest/gtest.h>
#include "VByte.h"

using namespace std;

TEST(VByteTest, encode) {
  uint64_t from = 1;
  uint8_t to[9] = {0};
  memset(to, 9, 0);
  uint32_t count_byte = VByte::encode(from, to);
  EXPECT_EQ(1, count_byte);
  EXPECT_EQ(0x81, to[0]);
  EXPECT_EQ(0, to[1]);
  EXPECT_EQ(0, to[2]);
  EXPECT_EQ(0, to[3]);
  EXPECT_EQ(0, to[4]);
  EXPECT_EQ(0, to[5]);
  EXPECT_EQ(0, to[6]);
  EXPECT_EQ(0, to[7]);

  memset(to, 9, 0);
  from = 127;
  count_byte = VByte::encode(from, to);
  EXPECT_EQ(1, count_byte);
  EXPECT_EQ(0xFF, to[0]);
  EXPECT_EQ(0, to[1]);
  EXPECT_EQ(0, to[2]);
  EXPECT_EQ(0, to[3]);
  EXPECT_EQ(0, to[4]);
  EXPECT_EQ(0, to[5]);
  EXPECT_EQ(0, to[6]);
  EXPECT_EQ(0, to[7]); EXPECT_EQ(0, to[1]);

  memset(to, 9, 0);
  from = 128;
  count_byte = VByte::encode(from, to);
  EXPECT_EQ(2, count_byte);
  EXPECT_EQ(0x00, to[0]);
  EXPECT_EQ(0x81, to[1]);
  EXPECT_EQ(0, to[2]);
  EXPECT_EQ(0, to[3]);
  EXPECT_EQ(0, to[4]);
  EXPECT_EQ(0, to[5]);
  EXPECT_EQ(0, to[6]);
  EXPECT_EQ(0, to[7]); 
}

TEST(VByteTest, test) {
  uint64_t from = 28038249122;
  uint8_t to[9];
  memset(to, 9, 0);
  uint32_t count_byte = VByte::encode(from, to);

  uint64_t decoded = 0;
  uint32_t byte_read = VByte::decode(&decoded, to);
  EXPECT_EQ(byte_read, count_byte);
  EXPECT_EQ(from, decoded);
}

inline void display(uint8_t *bytes, uint32_t len) {
  for (int i = 0; i < len; ++i) {
    cout << hex << (int)bytes[i] << endl;
  }
}

TEST(VByteTest, decode) {
  uint8_t from[9];

  memset(from, 0, 9);
  from[0] = 0x87;
  uint64_t value;
  uint32_t byte_count = VByte::decode(&value, from);
  EXPECT_EQ(1, byte_count);
  EXPECT_EQ(0x7, value);

  memset(from, 0, 9);
  from[0] = 0x7F;
  from[1] = 0x81;
  byte_count = VByte::decode(&value, from);
  EXPECT_EQ(2, byte_count);
  EXPECT_EQ(0xFF, value);

  memset(from, 0, 9);
  memset(from, 0x7f, 4);
  from[4] = 0x8f;
  //display(from, 9);
  byte_count = VByte::decode(&value, from);
  //cout << endl << "value" << endl;
  //display((uint8_t *)&value, sizeof(uint64_t));

  EXPECT_EQ(5, byte_count);
  uint64_t expected = ~0ULL >> 32;
  EXPECT_EQ(expected, value);
}
