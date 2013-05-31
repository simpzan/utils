#ifndef CONST_BIT_VECTOR_H
#define CONST_BIT_VECTOR_H

#include <stdint.h>
#include <limits.h>
#include <iostream>
#include "ConstVector.h"

#define BIT_PER_BYTE 8
#define BIT_PER_UNIT 64
#define UNIT_PER_BLOCK 4
#define BIT_PER_BLOCK (BIT_PER_UNIT*UNIT_PER_BLOCK)

class ConstBitVector {
 public:
  ConstBitVector() : _count_bit(0) {}
  virtual ~ConstBitVector() {}

  bool operator[](uint64_t offset) const;
  bool bitAt(uint64_t offset) const {  return operator[](offset);  }
  uint64_t count() const {  return _count_bit;  }

  uint64_t rank1(uint64_t id) const;
  uint64_t rank1Naive(uint64_t id) const;
  uint64_t rank0(uint64_t id) const {  return id + 1 - rank1(id);  }

  uint64_t select1Naive(uint64_t count) const;
  uint64_t select0Naive(uint64_t count) const;

  virtual void read(std::istream &is);
  virtual uint32_t mmap(const uint8_t *address);
  virtual void clear();
  virtual void display(std::ostream &os) const;

 protected:
  const ConstVector<uint8_t> &bytes() const {  return _bytes;  }

 private:
  uint64_t _count_bit;

  ConstVector<uint8_t> _bytes;
  ConstVector<uint32_t> _ranks_block;
  ConstVector<uint8_t> _ranks_unit;
};

#endif
