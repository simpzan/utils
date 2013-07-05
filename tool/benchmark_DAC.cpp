#include <iostream>
#include <vector>
#include <stdint.h>
#include <fstream>
#include "DACWrapper.h"
#include "timer.h"


using namespace std;

int main(int argc, const char *argv[])
{
  int count = 100000000;
  vector<uint32_t> list(count, 100);
  list[0] = 1111011163;
  list[1] = 0;
  list[2] = 255;
  list[3] = 15;

  Timer t;
  DACWrapper dac(list);
  t.Report();

  return 0;
}
