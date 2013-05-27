#ifndef BALANCED_BIT_VECTOR_BUILDER_H
#define BALANCED_BIT_VECTOR_BUILDER_H

#include "BitVectorBuilder.h"
#include "Vector.h"

class BalancedBitVectorBuilder : public BitVectorBuilder {
  public:
	BalancedBitVectorBuilder() {}
	virtual ~BalancedBitVectorBuilder() {}

	virtual void write(std::ostream &os);
	virtual void clear();

  static uint64_t sizeWithBitcount(uint64_t count);

  private:
	BitVectorBuilder _is_fars;
	BitVectorBuilder _is_pioneers;
	Vector<uint32_t> _pioneers;
};

#endif
