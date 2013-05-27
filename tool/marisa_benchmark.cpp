#include <iostream>
#include <marisa.h>
#include "timer.h"

using namespace std;

void benchmark(const marisa::Trie &trie) {
  const char *key = "test";
  marisa::Agent agent;
  agent.set_query(key, strlen(key));

  bool result;
  Timer timer;
  result = trie.lookup(agent);
  timer.Stop();
  cout << "cpu:" << timer.ElapsedTimeCPU() << "us" << endl;
  cout << "wall:" << timer.ElapsedTime() << "us" << endl;

  if (result) {
    cout << agent.key().id() << endl;
  } else {
    cout << key << " not found" << endl;
  } 
}

int main(int argc, const char *argv[])
{
  const char *idx_file = "../words.marisa";
  
  marisa::Trie *trie = new marisa::Trie;
  trie->mmap(idx_file);
  benchmark(*trie);
  delete trie;

  trie = new marisa::Trie;
  trie->load(idx_file);
  benchmark(*trie);
  delete trie;

  return 0;
}
