#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <vector>
#include <string>

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

typedef void (*Function)();
void benchmark(Function f, int count = 1);

int cmp(const char *l, const char *r);
int stringCompare(void const*, void const*);
bool isPrefix(const char *prefix, const char *key);

#endif
