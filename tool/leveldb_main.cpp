#include <iostream>
#include <assert.h>
#include <vector>
#include <string>

#include "utils.h"
#include "timer.h"
#include "leveldb/db.h"

using namespace std;

void loadTokens(const char *fname, vector<string> &tokens) {
  string file_content;
  bool result = loadfile(fname, file_content);
  assert(result);

  splitString((char*)file_content.data(), file_content.size(), '\n', tokens);
}

int main(int argc, const char *argv[])
{
  string input_file("words.sorted");

  leveldb::DB* db;
  leveldb::Options options;
  options.create_if_missing = true;
  string idx_name = input_file + ".leveldb";
  leveldb::Status status = leveldb::DB::Open(options, idx_name.c_str(), &db);
  assert(status.ok());

  vector<string> tokens;
  loadTokens(input_file.c_str(), tokens);

  char v[256];
  for (int i = 0; i < tokens.size(); ++i) {
    string token = tokens[i];
    sprintf(v, "%d", (int)token.size());
    string value(v);
    db->Put(leveldb::WriteOptions(), token, value);
  }

  int count = tokens.size();
  Timer t;
    string value;
  for (int i = 0; i < count; ++i) {
    string token = tokens[i];
    db->Get(leveldb::ReadOptions(), token, &value);
    //int key_len = 0;
    //sscanf(value.data(), "%d", &key_len);
    //cout << key_len << " " << token << endl;
    //assert(key_len == token.size());
  }
  t.Stop();
  cout << "cpu time(s):" << t.ElapsedTimeCPU() / 1000000
    << " avg(us):" << t.ElapsedTimeCPU()/count << endl
    << "wall time(s):" << t.ElapsedTime() / 1000000
    << " avg(us):" << t.ElapsedTime()/count << endl;
  delete db;
  return 0;
}
