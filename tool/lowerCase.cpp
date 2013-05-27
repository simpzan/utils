#include <fstream>
#include <iostream>
#include <cassert>
#include <string>

#include "utils.h"

using namespace std;

void toLowerCase(string &buf) {
	for (int bi=0; bi<buf.size(); ++bi) {
		buf[bi]	= tolower(buf[bi]);
	}
}

void save(const string &fname, uint8_t *buf, uint64_t bufSize) {
	ofstream os(fname.c_str());
	assert(os.is_open() && os.good());
	
	os.write((const char *)buf, bufSize);
}

int main() {
	const char *fname = "words";

  string file_content;
	bool load_result = loadfile(fname, file_content);
	assert(load_result);
	toLowerCase(file_content);
	
	string outFname = fname + string(".lowercase");
	save(outFname, (uint8_t *)file_content.data(), file_content.size());
	return 0;
}
