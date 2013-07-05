#include <iostream>
#include <vector>
#include <stdint.h>
#include <fstream>
#include "DACWrapper.h"
#include "timer.h"

using namespace std;

int main(int argc, const char *argv[])
{
  int count = 1000000;
  vector<uint32_t> list(count, 100);
  list[0] = 500000000;

  Timer t;
  DACWrapper dac(list);
  t.Report();

  return 0;
}
