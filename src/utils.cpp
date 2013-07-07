#include "utils.h"
#include <stdint.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>
#include <errno.h>
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
  uint32_t size_guess = len / 16;
  tokens.reserve(size_guess);

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

uint32_t sizeOfFile(const char *filename) {
  struct stat st;
  if (stat(filename, &st) == 0)
    return st.st_size;

  fprintf(stderr, "Cannot determine size of %s: %s\n",
      filename, strerror(errno));
  return -1;
}

uint32_t insertString(const string &str, vector<string> &strs) {
  vector<string>::iterator itr = find(strs.begin(), strs.end(), str);
  if (itr != strs.end()) {
    return itr - strs.begin() + 1;
  }
  strs.push_back(str);
  return strs.size();
}

void reverseKey(const char *key, string &reversed_key) {
  const uint8_t *akey = (const uint8_t *)key;
  int count = strlen(key);
  for (int i = 0; i < count; ++i) {
    reversed_key =(char) akey[i] + reversed_key;
  }
}

void shortestSeparator(const std::string &first, const std::string &second, 
    std::string &separator) {
  assert(first <= second);
  int len = computeLCP(first.c_str(), second.c_str());
  if (len == first.size()) {
    separator = first.substr(0, len);
    return;
  }

  if (len + 1 == first.size()) {
    separator = first.substr(0, len + 1);
    return;
  }

  separator = second.substr(0, len + 1);
}
