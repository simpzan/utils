#include <stdint.h>
#include <iostream>

using namespace std;

int offsetForByte(uint8_t byte, int e) {
  int count = 0;
  for (int i = 0; i < 8; ++i) {
    uint8_t mask = 1 << i;
    int diff = (byte & mask) ? 1 : -1;
    count += diff;
    if (count == e)  return i;
  }
  return e - count + 7;
}

void generateTableForExcess(int e) {
  for (int i = 0; i < 256; ++i) {
    if (i % 16 == 0 && i)  cout << endl;
    int offset = offsetForByte(i, e);
    fprintf(stdout, "%2d, ", offset);
  }
  cout << endl;
}

void generateTable() {
  for (int i = 1; i <= 8; ++i) {
    cout << "{" << endl;
    generateTableForExcess(i);
    cout << "}," << endl;
  }
}

int main(int argc, const char *argv[])
{
  generateTable();
  return 0;
}
