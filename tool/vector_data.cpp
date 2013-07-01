#include <iostream>
#include <sstream>
#include <vector>
#include "DACWrapper.h"

using namespace std;

int main(int argc, const char *argv[])
{
  int count = 8000000;
  vector<uint32_t> offsets(count);
  for (int i = 0; i < count; ++i) {
    offsets[i] = i*2+3;
  }
  
  DACWrapper dac(offsets.data(), count);
  stringstream ss;
  dac.write(ss);
  int size_encoded =  ss.str().size();
  int size_before = count * 4;
  float ratio = size_encoded / (float)size_before;
  cout << ratio << endl;

  return 0;
}
