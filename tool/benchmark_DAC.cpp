#include <iostream>
#include <vector>
#include <stdint.h>
#include <fstream>
#include "DACWrapper.h"
#include "timer.h"


using namespace std;

void timing(int count, uint32_t max) {
  cout << "count:" << count << " max:" << max << endl;
  vector<uint32_t> int_array(count);
  for (int i = 0; i < count; ++i) {
    int_array[i] = rand()  % 10000;
  }
  int_array[0] = max;
  int_array[1] = 1;
  
  Timer t;
  DACWrapper dac(int_array);
  t.Report();
}

int main(int argc, const char *argv[])
{
  timing(100, 100);
  timing(100, 100000000);
  timing(100000000, 100);
  timing(100000000, 100000000);
  return 0;
}
