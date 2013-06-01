#include <gtest/gtest.h>

#include <sstream>
#include "Vector.h"
#include "ConstVector.h"

using namespace std;

TEST(VectorTest, erase) {
  Vector<char> v;
  for (int i = 0; i < 20; ++i) {
    v.append(i);
  }
  v.erase(0, 10);
  for (int i = 0; i < 10; ++i) {
    int expected = i + 10;
    EXPECT_EQ(expected, v[i]);
  }
}

TEST(VectorTest, append) {
  Vector<char> v;
  v.append('a');
  EXPECT_EQ('a', v[0]);

  uint64_t count = v.count();
  EXPECT_EQ(1, count);

  uint64_t size = v.size();
  EXPECT_GE(size, 0);

  v.clear();
  count = v.count();
  EXPECT_EQ(0, count);
}

TEST(VectorTest, readWrite) {
  Vector<char> v;
  v.append('a');
  v.append('Z');

  std::stringstream ss;
  v.write(ss);

  Vector<char> v2;
  v2.read(ss);
  uint64_t count = v2.count();
  EXPECT_EQ(2, count);
  EXPECT_EQ('Z', v2[1]);
}

TEST(VectorTest, ConstVectorTest) {
  Vector<uint64_t> v;
  v.append(22);
  v.append(44);

  stringstream ss;
  v.write(ss);

  ConstVector<uint64_t> cv;
  cv.read(ss);

  EXPECT_EQ(2, cv.count());
  EXPECT_EQ(22, cv[0]);
  EXPECT_EQ(44, cv[1]);
}

TEST(VectorTest, mmap) {
  Vector<uint64_t> v;
  v.append(22);
  v.append(44);

  stringstream ss;
  v.write(ss);

  ConstVector<uint64_t> cv;
  const uint8_t *bytes = (uint8_t *)ss.str().c_str();
  uint64_t consumed_size = cv.mmap(bytes);
  uint64_t expected = cv.size();
  EXPECT_EQ(expected, consumed_size);

  EXPECT_EQ(2, cv.count());
  EXPECT_EQ(22, cv[0]);
  EXPECT_EQ(44, cv[1]); 
}
