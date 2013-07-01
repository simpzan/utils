#ifndef UTILS_H
#define UTILS_H

#include <cstring>
#include <stdint.h>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

// load the content of the file into |file_content|.
bool loadfile(const char *filename, std::string &file_content);

// load the tokens from file, tokens are separated by newline.
bool loadTokensFromFile(const std::string &filename, 
    std::vector<std::string> &tokens);

// tokenize the string separated by split char.
// str and len: source string to split.
// split: the split char.
// tokens: outut tokens splitted from the string.
void splitString(const char *str, uint32_t len, char split, 
    std::vector<std::string> &tokens);

uint8_t computeLCP(const char *lastKey, const char *thisKey);

int cmp(const char *l, const char *r);
int stringCompare(void const*, void const*);
bool isPrefix(const char *prefix, const char *key);

uint32_t sizeOfFile(const char *filename);

uint32_t insertString(const std::string &str, std::vector<std::string> &strs);
void reverseKey(const char *key, std::string &reversed_key);

inline bool stringStartsWtih(const char *str_long, const char *str_short) {
  int lcp = computeLCP(str_long, str_short);
  return lcp == strlen(str_short);
}
inline void showOffset(std::istream &is) {
  uint32_t offset = is.tellg();
  std::cout << "offset: " << offset << std::endl;
}

inline void showOffset(std::ostream &os) {
  uint32_t offset = os.tellp();
  std::cout << "offset: " << offset << std::endl;
}

template <class T>
double ratio(const T &rank)  {
  uint32_t bitCount = rank.size();
  uint32_t ones = rank.rank(bitCount - 1);
  double r = (double)ones/ bitCount;

  return r;
}

void shortestSeparator(const std::string &first, const std::string &second, 
    std::string &separator);

#endif
