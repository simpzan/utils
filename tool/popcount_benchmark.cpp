#include <stdint.h>
#include <iostream>

#include "timer.h"

using namespace std;

template <typename T>
int countOne2(T v) {
  v = v - ((v >> 1) & (T)~(T)0/3);                           // temp
  v = (v & (T)~(T)0/15*3) + ((v >> 2) & (T)~(T)0/15*3);      // temp
  v = (v + (v >> 4)) & (T)~(T)0/255*15;                      // temp
  int c = (T)(v * ((T)~(T)0/255)) >> (sizeof(T) - 1) * CHAR_BIT; // count
  return c;
}

uint8_t countOne(uint64_t unit) {
  unit = ((unit & 0xAAAAAAAAAAAAAAAAULL) >> 1)
    + (unit & 0x5555555555555555ULL);
  unit = ((unit & 0xCCCCCCCCCCCCCCCCULL) >> 2)
    + (unit & 0x3333333333333333ULL);
  unit = ((unit >> 4) + unit) & 0x0F0F0F0F0F0F0F0FULL;
  unit += unit << 8;
  unit += unit << 16;
  unit += unit << 32;
  unit >>= 56;
  return unit;
}


static unsigned char BitsSetTable256[256] = {
#   define B2(n) n,     n+1,     n+1,     n+2
#   define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#   define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
  B6(0), B6(1), B6(1), B6(2)
};

static bool g_table_initialized = false;

// To initially generate the table algorithmically:
void initializeTable() {
  BitsSetTable256[0] = 0;
  for (int i = 0; i < 256; i++) {
    BitsSetTable256[i] = (i & 1) + BitsSetTable256[i / 2];
  }
  g_table_initialized = true;
}

uint32_t countOneTable(uint64_t unit) {
  uint8_t *p = (uint8_t *) &unit;
  return BitsSetTable256[p[0]] + 
    BitsSetTable256[p[1]] + 
    BitsSetTable256[p[2]] +	
    BitsSetTable256[p[3]] +
    BitsSetTable256[p[4]] +
    BitsSetTable256[p[5]] +
    BitsSetTable256[p[6]] +
    BitsSetTable256[p[7]];
}

int main(int argc, const char *argv[])
{
  uint64_t unit =323423423903423425;
  int result = 0;
  int times = 100000;
  Timer timer;
  for (int i = 0; i < times; ++i) {
    result = countOne(unit);
  }
  timer.Stop();
  cout << "resut:" << result << " cpu time:" << timer.ElapsedTimeCPU() << endl;

  initializeTable();
  timer.Restart();
  for (int i = 0; i < times; ++i) {
    result = countOneTable(unit);
  }
  timer.Stop();
  cout << "resut:" << result << " cpu time:" << timer.ElapsedTimeCPU() << endl;

  timer.Restart();
  for (int i = 0; i < times; ++i) {
    result = countOne2(unit);
  }
  timer.Stop();
  cout << "resut:" << result << " cpu time:" << timer.ElapsedTimeCPU() << endl;


  return 0;
}
