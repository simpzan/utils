#include "utils.h"
#include <stdint.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "timer.h"

using namespace std;

bool loadfile(const char *file, string &file_content) {
  ifstream is(file, iostream::in);
  assert(is.good());

  // get filesize.
  is.seekg(0, iostream::end);
  uint64_t filesize = is.tellg();
  is.seekg(0, iostream::beg);

  // reserve memory.
  file_content.resize(filesize);

  // read data.
  is.read((char *)file_content.data(), filesize);
  if(!is.good()) {
    cout << "read failed" << endl;
  }
  is.close();

  cout << "filesize: " << file << "=" << filesize << endl;
  return true;
}

void splitString(const char *str, uint32_t len, char split, 
    std::vector<std::string> &tokens) { 
  int previousI = 0;

  for (uint32_t si=0; si<len; ++si ) {
    if (str[si] == split) {
      int token_len = si - previousI;
      string token(str+previousI, token_len);
      tokens.push_back(token);

      previousI = si+1;
    }
  }

  int tlen = len - previousI;
  if (tlen) {
    string token(str + previousI, tlen);
    tokens.push_back(token);
  }
}

bool loadTokensFromFile(const string &filename, vector<string> &tokens) { 
  tokens.clear();
  string file_content;
  bool result = loadfile(filename.c_str(), file_content);
  if (!result) {
    return false;
  }

  splitString(file_content.data(), file_content.size(), '\n', tokens);
  return true;
}

int cmp(const char *l, const char *r) {
  int diff =0;
  while (*l && *r) {
    diff = tolower(*l) - tolower(*r);
    if (diff) {
      return diff;
    }
    ++l;
    ++r;
  }
  if (*l) {
    return +1;
  } else {
    return -1;
  }
}

int stringCompare(const void *arg1, const void *arg2) {
  const char *str1 = * (const char **) arg1;
  const char *str2 = * (const char **) arg2;
  int returnValue = strcmp(str1, str2);
  return returnValue;
}

uint8_t computeLCP(const char *lastKey, const char *thisKey) {
  int count = min(strlen(lastKey), strlen(thisKey));
  for (int i = 0; i < count; ++i) {
    if (lastKey[i] != thisKey[i])  return i;
  }
  return count;
}
