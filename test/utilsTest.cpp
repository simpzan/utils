#include <string>
#include <vector>
#include <gtest/gtest.h>
#include "utils.h"

using namespace std;

TEST(utilsTest, test) {
	char cstring[] = "this\nis\na\ntest";
	
	vector<string> tokens;
	splitString(cstring, strlen(cstring), '\n', tokens);
	
	assert(tokens.size() == 4);
	assert(tokens[0] == string("this"));
	assert(tokens[3] == string("test"));
}

TEST(utilsTest, loadfile) {
  string file_content;
  const char *file = "/usr/share/dict/words";
  bool result = loadfile(file, file_content);
  EXPECT_TRUE(result);
  EXPECT_EQ(2493109, file_content.size());
}

TEST(utilsTest, loadTokens) {
  vector<string> tokens;
  string filename("words.sorted");
  bool result = loadTokensFromFile(filename, tokens);
  EXPECT_TRUE(result);
  EXPECT_EQ(235886, tokens.size());
}

TEST(utilsTest, separator) {
  string first("Alexis");
  string second("Antimason");
  string separator;
  shortestSeparator(first, second, separator);
  EXPECT_TRUE(first <= separator && separator < second);
  EXPECT_TRUE(separator.size() == 2);

  first = "test";
  second = "tester";
  shortestSeparator(first, second, separator);
  EXPECT_EQ(first, separator);
}

