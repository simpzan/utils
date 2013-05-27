#ifndef CONST_BALANCED_BIT_VECTOR_H
#define CONST_BALANCED_BIT_VECTOR_H

#include "ConstBitVector.h"
#include "ConstVector.h"

class ConstBalancedBitVector : public ConstBitVector {
 public:
  ConstBalancedBitVector() {}
  virtual ~ConstBalancedBitVector() {}

  uint64_t findClose(uint64_t id) const;
  uint64_t findCloseNaive(uint64_t id) const;
  uint64_t findNextClose(uint64_t id) const;
  uint64_t findOpenNaive(uint64_t id) const;
  uint64_t excess(uint64_t id) const;
  uint64_t encloseNaive(uint64_t id) const;

  virtual void read(std::istream &is);
  virtual uint32_t mmap(const uint8_t *address);
  virtual void clear();
  virtual void display(std::ostream &os) const;
  void display() const;

 private:
  uint64_t _findCloseFar(uint64_t id) const;
  int32_t _findCloseNear(uint64_t id) const;

 private:
  ConstBitVector _is_fars;
  ConstBitVector _is_pioneers;
  ConstVector<uint32_t> _pioneers;
};

#endif

