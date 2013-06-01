#ifndef CONST_BALANCED_BIT_VECTOR_H
#define CONST_BALANCED_BIT_VECTOR_H

#include "ConstBitVector.h"
#include "ConstVector.h"

class ConstBalancedBitVector : public ConstBitVector {
 public:
  ConstBalancedBitVector() {}
  virtual ~ConstBalancedBitVector() {}

  uint32_t findClose(uint32_t id) const;
  uint32_t findCloseNaive(uint32_t id) const;
  uint32_t findNextClose(uint32_t id) const;
  uint32_t findOpenNaive(uint32_t id) const;
  uint32_t excess(uint32_t id) const;
  uint32_t encloseNaive(uint32_t id) const;

  virtual void read(std::istream &is);
  virtual uint32_t mmap(const uint8_t *address);
  virtual void clear();
  virtual void display(std::ostream &os) const;
  void display() const;

 private:
  uint32_t _findCloseFar(uint32_t id) const;
  int32_t _findCloseNear(uint32_t id) const;

 private:
  ConstBitVector _is_fars;
  ConstBitVector _is_pioneers;
  ConstVector<uint32_t> _pioneers;
};

#endif

