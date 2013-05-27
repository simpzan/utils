#include <cassert>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <fstream>

#include "utils.h"

using namespace std;

bool cmp(const string &a, const string &b) {
	
	return true;
}

void saveTokens(vector<string> &tokens, const string &fname) {
	ostringstream oss;
	for (int ti=0; ti<tokens.size(); ++ti) {
		oss << tokens[ti] << endl;
	}

	ofstream os(fname.c_str());
	os << oss.str();
	
}

int main() {
	const char *fname = "words";

  string buf;
	bool result = loadfile(fname, buf);
	assert(result);

	vector<string> tokens;
  uint64_t bufSize = buf.size();
	splitString((char *)buf.data(), bufSize, '\n', tokens);

	stable_sort(tokens.begin(), tokens.end());
	
	string outFname = fname + string(".sorted");
	saveTokens(tokens, outFname);

	return 0;
}
