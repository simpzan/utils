#include <iostream>
#include <sstream>
#include <bitset>
#include <sdsl/int_vector.hpp>
#include <sdsl/util.hpp>
#include "BitVectorBuilder.h"
#include "ConstBitVector.h"
#include "BalancedBitVectorBuilder.h"
#include "ConstBalancedBitVector.h"
#include "timer.h"

using namespace std;
using namespace sdsl;

void bp() {
  BalancedBitVectorBuilder builder;
  int count = 4*1024;
  for (int i = 0; i < count; ++i) {
    builder.append(0);
  }
  for (int i = 0; i < count; ++i) {
    builder.append(1);
  }
  
  stringstream ss;
  builder.write(ss);

  ConstBalancedBitVector bp;
  bp.read(ss);
  
  cout << "size: " << (int)ss.str().size() << endl;
  Timer timer;
  bp.findClose(100);
  timer.Stop();
  cout << "rank time(us):" << timer.ElapsedTimeCPU() << endl;
}

int main(int argc, const char *argv[]) {
  const int count = 8 * 1024;
  
  bitset<count> bs;
  for (int i = 0; i < count; ++i) {
    bs.set(rand() % count);
  }
  
  BitVectorBuilder builder;
  for (int i = 0; i < count; ++i) {
    bool bit = bs[i];
    builder.append(bit);
  }
  stringstream ss;
  builder.write(ss);
  int len = ss.str().size();
  cout << "my implementation len:" << len << endl;

  ConstBitVector cbv;
  cbv.read(ss);

  int result = 0;
  int pos = count / 2;
  Timer timer;
  result = cbv.rank1(pos);
  timer.Stop();
  cout << "rank time(us):" << timer.ElapsedTimeCPU() << endl;

  bp();
  return 0;
}
