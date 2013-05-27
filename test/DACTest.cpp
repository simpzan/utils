#include <iostream>
#include <sstream>
#include <gtest/gtest.h>
#include "DACWrapper.h"

using namespace std;

TEST(DACTest, test) {
  int count = 1000000;
  uint32_t int_array[count];
  for (int i = 0; i < count; ++i) {
    int_array[i] = i;
  }
  
  DACWrapper dac(int_array, count);
  for (int i = 0; i < count; ++i) {
    EXPECT_EQ(int_array[i], dac[i]);
  }
  
  stringstream ss;
  dac.write(ss);
  DACWrapper d2;
  d2.read(ss);
  for (int i = 0; i < count; ++i) {
    EXPECT_EQ(int_array[i], d2[i]);
  }
  
}
