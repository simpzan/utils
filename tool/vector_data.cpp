#include <iostream>
#include <vector>

using namespace std;

int main(int argc, const char *argv[])
{
  vector<int> iv;
  iv.reserve(10);
  cout << iv.size() << endl;
  int *p = iv.data();
  p[9] = 10;
  cout << iv.size() << " " << iv[9] << endl;
  iv.resize(10);
  cout << iv.size() << " " << iv[9] << endl;
  return 0;
}
