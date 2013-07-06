#include <iostream>
#include <sstream>
#include <gtest/gtest.h>
#include "DACWrapper.h"
#include "timer.h"
#include "Vector.h"

using namespace std;

TEST(DACTest, debug) {
  ifstream is("data_test/DAC.bug");
  assert(is.good());
  Vector<uint32_t> links;
  links.read(is);

  DACWrapper dac(links.vector());

  int count = links.count();
  for (int i = 0; i < count; ++i) {
    ASSERT_EQ(links[i], dac[i]) << i;
  }
}

TEST(DACTest, test) {
  int count = 1000000;
  uint32_t int_array[count];
  for (int i = 0; i < count; ++i) {
    int_array[i] = rand()  % 10000;
  }
  int_array[0] = 1000000;
  int_array[1] = 1;
  
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

TEST(DACTest, benchmark) {
  long long count = 1*1000*1000;
  uint32_t *int_array = new uint32_t[count];
  for (long i = 0; i < count; ++i) {
    int_array[i] = i % 10;
  }
  
  Timer timer;
  DACWrapper dac(int_array, count);
  timer.Stop();
  cout << timer.ElapsedTimeCPU() << endl;

  for (int i = 0; i < count; ++i) {
    EXPECT_EQ(int_array[i], dac[i]);
  }

  int kk;
  timer.Restart();
  for (int i = 0; i < count; ++i) {
    kk = int_array[i];
  }
  timer.Stop();
  cout << timer.ElapsedTimeCPU() << endl;

  timer.Restart();
  for (int i = 0; i < count; ++i) {
    dac[i];
  }
  timer.Stop();
  cout << timer.ElapsedTimeCPU() << endl;

  stringstream ss;
  dac.write(ss);
  int size_compressed = ss.str().size();
  int size_original = count * sizeof(uint32_t);
  cout << size_original << "->" << size_compressed << endl;
  cout << (float)size_compressed/size_original << endl;
  delete int_array;
}
