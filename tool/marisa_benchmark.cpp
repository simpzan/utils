#include <iostream>
#include <string>
#include <vector>
#include <marisa.h>
#include "utils.h"
#include "timer.h"

using namespace std;

void benchmark(const marisa::Trie &trie, const vector<string> &tokens) {
  const char *key = "test";
  marisa::Agent agent;
  agent.set_query(key, strlen(key));

  getchar();
  bool result;
  Timer timer;
  int count = tokens.size() / 2;
  for (int i = 0; i < count; ++i) {
    string token = tokens[i];
    agent.set_query(token.c_str(), token.size());
    result = trie.lookup(agent);
  }
  timer.Stop();
  cout << "cpu:" << timer.ElapsedTimeCPU() << "us" << endl;
  cout << "wall:" << timer.ElapsedTime() << "us" << endl;

  if (result) {
    cout << agent.key().id() << endl;
  } else {
    cout << key << " not found" << endl;
  } 
  getchar();
}

int main(int argc, const char *argv[])
{
  const char *token_file = "../enwiki";
  vector<string> tokens;
  loadTokensFromFile(token_file, tokens);
  
  const char *idx_file = "../enwiki.marisa";
  marisa::Trie *trie;
  trie = new marisa::Trie;
  trie->mmap(idx_file);
  benchmark(*trie, tokens);
  delete trie;

  //trie = new marisa::Trie;
  //trie->load(idx_file);
  //benchmark(*trie, tokens);
  //delete trie;

  return 0;
}
