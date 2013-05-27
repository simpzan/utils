#ifndef BIT_VECTOR_BUILDER_H
#define BIT_VECTOR_BUILDER_H

#include <vector>
#include <stdint.h>
#include <iostream>
#include "Vector.h"
#include "ConstBitVector.h"

class BitVectorBuilder 
{
public:
	BitVectorBuilder() : _count_one(0), _rank_block(0) {}
	virtual ~BitVectorBuilder() {}

	virtual void append(bool bit);
	virtual void write(std::ostream &os);

	uint64_t size();
  static uint64_t sizeWithBitcount(uint32_t count);
	uint64_t count()  {  return _bits.size();  }

	virtual void clear();
	void display(std::ostream &os);

protected:
	std::vector<bool> &bits() {  return _bits;  }

private:
	void appendRank(bool bit);
	void writeBoolVector(std::ostream &os, const std::vector<bool> &bits);

private:
	std::vector<bool> _bits;
	Vector<uint32_t> _ranks_block;
	Vector<uint8_t> _ranks_unit;

	uint32_t _count_one;
	uint32_t _rank_block; // rank1 of the start of the block.
};

inline static uint64_t sizeWithBitcount(uint32_t count) {
  uint64_t size = 0;

  return size;
}

#endif
