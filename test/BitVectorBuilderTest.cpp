#include <gtest/gtest.h>
#include <sstream>

#include "BitVectorBuilder.h"
#include "ConstBitVector.h"

using namespace std;

class BitVectorTest: public ::testing::Test {
  protected:
	BitVectorTest() {
		bvb.append(false);

		bvb.append(false);
		bvb.append(false);
		bvb.append(false);
		bvb.append(true);

		bvb.append(false);
		bvb.append(true);

		bvb.append(true);

		bvb.append(false);
		bvb.append(false);
		bvb.append(true);

		bvb.append(true);

		bvb.append(true);

		bvb.append(true);

		stringstream ss;
		bvb.write(ss);
		cbv.read(ss);
	}
	~BitVectorTest() {}

	BitVectorBuilder bvb;
	ConstBitVector cbv;
};

TEST_F(BitVectorTest, rank) {
 	uint64_t offset = 3;
	uint64_t count = cbv.rank1(offset);
	uint64_t expected = 0;
	EXPECT_EQ(expected, count);

	offset = 4;
	count = cbv.rank1(offset);
	expected = 1;
	EXPECT_EQ(expected, count);

	offset = 6;
	count = cbv.rank0(offset);
	expected = 5;
	EXPECT_EQ(expected, count);
}

TEST_F(BitVectorTest, select) {
	uint64_t count = 1;
	uint64_t offset = cbv.select1Naive(count);
	uint64_t expected = 4;
	EXPECT_EQ(expected, offset);

	count = 5;
	offset = cbv.select0Naive(count);
	expected = 5;
	EXPECT_EQ(expected, offset);
}

TEST_F(BitVectorTest, access) {
	EXPECT_EQ(false, cbv[0]);
	EXPECT_EQ(true, cbv[4]);
	EXPECT_EQ(true, cbv[6]);
	EXPECT_EQ(true, cbv[7]);
	EXPECT_EQ(false, cbv[8]);
	EXPECT_EQ(true, cbv[10]);
	EXPECT_EQ(true, cbv[13]);
}


TEST(BitVectorTest2, rank) {
	BitVectorBuilder builder;
	for (int i = 0; i < 801; ++i) {
		builder.append(false);
	}
	for (int i = 0; i < 801; ++i) {
		builder.append(true);
	}
	stringstream ss;
	builder.write(ss);
	ConstBitVector bv;
	bv.read(ss);

	uint64_t id = 0;
	uint64_t count = bv.rank1(id);
	uint64_t expected = 0;
	EXPECT_EQ(expected, count);
	count = bv.rank0(id);
	expected = 1;
	EXPECT_EQ(expected, count);

	id = 800;
	count = bv.rank1(id);
	expected = 0;
	EXPECT_EQ(expected, count);
	count = bv.rank0(id);
	expected = 801;
	EXPECT_EQ(expected, count);

	id = 1601;
	count = bv.rank1(id);
	expected = 801;
	EXPECT_EQ(expected, count);
	count = bv.rank0(id);
	EXPECT_EQ(expected, count);
}

TEST(BitVectorTest2, rank2) {
	BitVectorBuilder builder;
	for (int i = 0; i < 802; ++i) {
		builder.append(false);
		builder.append(true);
	}
	stringstream ss;
	builder.write(ss);
	ConstBitVector bv;
	bv.read(ss);

	uint64_t id = 0;
	uint64_t count = bv.rank1(id);
	uint64_t expected = 0;
	EXPECT_EQ(expected, count);
	count = bv.rank0(id);
	expected = 1;
	EXPECT_EQ(expected, count);

	id = 801;
	count = bv.rank1(id);
	expected = 401;
	EXPECT_EQ(expected, count);
	count = bv.rank0(id);
	EXPECT_EQ(expected, count);

	id = 1603;
	count = bv.rank1(id);
	expected = 802;
	EXPECT_EQ(expected, count);
	count = bv.rank0(id);
	EXPECT_EQ(expected, count);
}
