#include <gtest/gtest.h>
#include <sstream>

#include "BalancedBitVectorBuilder.h"
#include "ConstBalancedBitVector.h"

using namespace std;

class BalancedBitVectorTest: public ::testing::Test {
  protected:
	BalancedBitVectorTest() {
		for (int i = 0; i < 801; ++i) {
			bvb.append(false);
			bvb.append(true);
		}
		stringstream ss;
		bvb.write(ss);
		cbv.read(ss);
	}
	~BalancedBitVectorTest() {}

	BalancedBitVectorBuilder bvb;
	ConstBalancedBitVector cbv;
};


TEST_F(BalancedBitVectorTest, findClose) {
	uint64_t offset = 0;
	uint64_t offset_close = 0;
	uint64_t expected = 0;

	for (int i = 0; i < 1602; i+=2) {
		offset_close = cbv.findClose(i);
		expected = cbv.findCloseNaive(i);
		ASSERT_EQ(expected, offset_close) << "i:" << i << endl;
	}
	
}

TEST(BalancedBitVectorTest2, shortTest) {
	BalancedBitVectorBuilder builder;
	builder.append(false);
	builder.append(false);
	builder.append(true);
	builder.append(true);
	stringstream ss;
	builder.write(ss);
	ConstBalancedBitVector bv;
	bv.read(ss);
	uint64_t result = bv.findClose(0);
	EXPECT_EQ(3, result);
	result = bv.findClose(1);
	EXPECT_EQ(2, result);
}

TEST_F(BalancedBitVectorTest, findOpen) {
	uint64_t offset = 1601;
	uint64_t expected = 1600;
	uint64_t openOffset = cbv.findOpenNaive(offset);
	EXPECT_EQ(expected, openOffset);

	offset = 1;
	expected = 0;
	openOffset = cbv.findOpenNaive(offset);
	EXPECT_EQ(expected, openOffset);
}
