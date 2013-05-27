#include <iostream>
#include <sstream>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/util.hpp>
#include "BitVectorBuilder.h"

using namespace std;
using namespace sdsl;

int main(int argc, const char *argv[])
{
  int count = 8 * 1024;
  BitVectorBuilder builder;
  for (int i = 0; i < count; ++i) {
    bool bit = i % 5 == 0;
    builder.append(bit);
  }
  stringstream ss;
  builder.write(ss);
  int len = ss.str().size();
  cout << "my implementation len:" << len << endl;

  return 0;
}
