#include "ConstBalancedBitVector.h"

using namespace std;

namespace {

// [excess e-1][a byte B]. so the table return the offset(0-based) of B on which
// the right excess is e.
static const uint8_t _table[8][256] = {
  {
    16,  0, 14,  0, 14,  0,  2,  0, 14,  0, 12,  0, 12,  0,  2,  0,
    14,  0, 12,  0, 12,  0,  2,  0, 12,  0,  4,  0,  4,  0,  2,  0,
    14,  0, 12,  0, 12,  0,  2,  0, 12,  0, 10,  0, 10,  0,  2,  0,
    12,  0, 10,  0, 10,  0,  2,  0, 10,  0,  4,  0,  4,  0,  2,  0,
    14,  0, 12,  0, 12,  0,  2,  0, 12,  0, 10,  0, 10,  0,  2,  0,
    12,  0, 10,  0, 10,  0,  2,  0, 10,  0,  4,  0,  4,  0,  2,  0,
    12,  0, 10,  0, 10,  0,  2,  0, 10,  0,  6,  0,  6,  0,  2,  0,
    10,  0,  6,  0,  6,  0,  2,  0,  6,  0,  4,  0,  4,  0,  2,  0,
    14,  0, 12,  0, 12,  0,  2,  0, 12,  0, 10,  0, 10,  0,  2,  0,
    12,  0, 10,  0, 10,  0,  2,  0, 10,  0,  4,  0,  4,  0,  2,  0,
    12,  0, 10,  0, 10,  0,  2,  0, 10,  0,  8,  0,  8,  0,  2,  0,
    10,  0,  8,  0,  8,  0,  2,  0,  8,  0,  4,  0,  4,  0,  2,  0,
    12,  0, 10,  0, 10,  0,  2,  0, 10,  0,  8,  0,  8,  0,  2,  0,
    10,  0,  8,  0,  8,  0,  2,  0,  8,  0,  4,  0,  4,  0,  2,  0,
    10,  0,  8,  0,  8,  0,  2,  0,  8,  0,  6,  0,  6,  0,  2,  0,
    8,  0,  6,  0,  6,  0,  2,  0,  6,  0,  4,  0,  4,  0,  2,  0,
  },
  {
    17, 15, 15,  1, 15, 13, 13,  1, 15, 13, 13,  1, 13,  3,  3,  1,
    15, 13, 13,  1, 13, 11, 11,  1, 13, 11, 11,  1, 11,  3,  3,  1,
    15, 13, 13,  1, 13, 11, 11,  1, 13, 11, 11,  1, 11,  3,  3,  1,
    13, 11, 11,  1, 11,  5,  5,  1, 11,  5,  5,  1,  5,  3,  3,  1,
    15, 13, 13,  1, 13, 11, 11,  1, 13, 11, 11,  1, 11,  3,  3,  1,
    13, 11, 11,  1, 11,  9,  9,  1, 11,  9,  9,  1,  9,  3,  3,  1,
    13, 11, 11,  1, 11,  9,  9,  1, 11,  9,  9,  1,  9,  3,  3,  1,
    11,  9,  9,  1,  9,  5,  5,  1,  9,  5,  5,  1,  5,  3,  3,  1,
    15, 13, 13,  1, 13, 11, 11,  1, 13, 11, 11,  1, 11,  3,  3,  1,
    13, 11, 11,  1, 11,  9,  9,  1, 11,  9,  9,  1,  9,  3,  3,  1,
    13, 11, 11,  1, 11,  9,  9,  1, 11,  9,  9,  1,  9,  3,  3,  1,
    11,  9,  9,  1,  9,  5,  5,  1,  9,  5,  5,  1,  5,  3,  3,  1,
    13, 11, 11,  1, 11,  9,  9,  1, 11,  9,  9,  1,  9,  3,  3,  1,
    11,  9,  9,  1,  9,  7,  7,  1,  9,  7,  7,  1,  7,  3,  3,  1,
    11,  9,  9,  1,  9,  7,  7,  1,  9,  7,  7,  1,  7,  3,  3,  1,
    9,  7,  7,  1,  7,  5,  5,  1,  7,  5,  5,  1,  5,  3,  3,  1,
  },
  {
    18, 16, 16, 14, 16, 14, 14,  2, 16, 14, 14, 12, 14, 12, 12,  2,
    16, 14, 14, 12, 14, 12, 12,  2, 14, 12, 12,  4, 12,  4,  4,  2,
    16, 14, 14, 12, 14, 12, 12,  2, 14, 12, 12, 10, 12, 10, 10,  2,
    14, 12, 12, 10, 12, 10, 10,  2, 12, 10, 10,  4, 10,  4,  4,  2,
    16, 14, 14, 12, 14, 12, 12,  2, 14, 12, 12, 10, 12, 10, 10,  2,
    14, 12, 12, 10, 12, 10, 10,  2, 12, 10, 10,  4, 10,  4,  4,  2,
    14, 12, 12, 10, 12, 10, 10,  2, 12, 10, 10,  6, 10,  6,  6,  2,
    12, 10, 10,  6, 10,  6,  6,  2, 10,  6,  6,  4,  6,  4,  4,  2,
    16, 14, 14, 12, 14, 12, 12,  2, 14, 12, 12, 10, 12, 10, 10,  2,
    14, 12, 12, 10, 12, 10, 10,  2, 12, 10, 10,  4, 10,  4,  4,  2,
    14, 12, 12, 10, 12, 10, 10,  2, 12, 10, 10,  8, 10,  8,  8,  2,
    12, 10, 10,  8, 10,  8,  8,  2, 10,  8,  8,  4,  8,  4,  4,  2,
    14, 12, 12, 10, 12, 10, 10,  2, 12, 10, 10,  8, 10,  8,  8,  2,
    12, 10, 10,  8, 10,  8,  8,  2, 10,  8,  8,  4,  8,  4,  4,  2,
    12, 10, 10,  8, 10,  8,  8,  2, 10,  8,  8,  6,  8,  6,  6,  2,
    10,  8,  8,  6,  8,  6,  6,  2,  8,  6,  6,  4,  6,  4,  4,  2,
  },
  {
    19, 17, 17, 15, 17, 15, 15, 13, 17, 15, 15, 13, 15, 13, 13,  3,
    17, 15, 15, 13, 15, 13, 13, 11, 15, 13, 13, 11, 13, 11, 11,  3,
    17, 15, 15, 13, 15, 13, 13, 11, 15, 13, 13, 11, 13, 11, 11,  3,
    15, 13, 13, 11, 13, 11, 11,  5, 13, 11, 11,  5, 11,  5,  5,  3,
    17, 15, 15, 13, 15, 13, 13, 11, 15, 13, 13, 11, 13, 11, 11,  3,
    15, 13, 13, 11, 13, 11, 11,  9, 13, 11, 11,  9, 11,  9,  9,  3,
    15, 13, 13, 11, 13, 11, 11,  9, 13, 11, 11,  9, 11,  9,  9,  3,
    13, 11, 11,  9, 11,  9,  9,  5, 11,  9,  9,  5,  9,  5,  5,  3,
    17, 15, 15, 13, 15, 13, 13, 11, 15, 13, 13, 11, 13, 11, 11,  3,
    15, 13, 13, 11, 13, 11, 11,  9, 13, 11, 11,  9, 11,  9,  9,  3,
    15, 13, 13, 11, 13, 11, 11,  9, 13, 11, 11,  9, 11,  9,  9,  3,
    13, 11, 11,  9, 11,  9,  9,  5, 11,  9,  9,  5,  9,  5,  5,  3,
    15, 13, 13, 11, 13, 11, 11,  9, 13, 11, 11,  9, 11,  9,  9,  3,
    13, 11, 11,  9, 11,  9,  9,  7, 11,  9,  9,  7,  9,  7,  7,  3,
    13, 11, 11,  9, 11,  9,  9,  7, 11,  9,  9,  7,  9,  7,  7,  3,
    11,  9,  9,  7,  9,  7,  7,  5,  9,  7,  7,  5,  7,  5,  5,  3,
  },
  {
    20, 18, 18, 16, 18, 16, 16, 14, 18, 16, 16, 14, 16, 14, 14, 12,
    18, 16, 16, 14, 16, 14, 14, 12, 16, 14, 14, 12, 14, 12, 12,  4,
    18, 16, 16, 14, 16, 14, 14, 12, 16, 14, 14, 12, 14, 12, 12, 10,
    16, 14, 14, 12, 14, 12, 12, 10, 14, 12, 12, 10, 12, 10, 10,  4,
    18, 16, 16, 14, 16, 14, 14, 12, 16, 14, 14, 12, 14, 12, 12, 10,
    16, 14, 14, 12, 14, 12, 12, 10, 14, 12, 12, 10, 12, 10, 10,  4,
    16, 14, 14, 12, 14, 12, 12, 10, 14, 12, 12, 10, 12, 10, 10,  6,
    14, 12, 12, 10, 12, 10, 10,  6, 12, 10, 10,  6, 10,  6,  6,  4,
    18, 16, 16, 14, 16, 14, 14, 12, 16, 14, 14, 12, 14, 12, 12, 10,
    16, 14, 14, 12, 14, 12, 12, 10, 14, 12, 12, 10, 12, 10, 10,  4,
    16, 14, 14, 12, 14, 12, 12, 10, 14, 12, 12, 10, 12, 10, 10,  8,
    14, 12, 12, 10, 12, 10, 10,  8, 12, 10, 10,  8, 10,  8,  8,  4,
    16, 14, 14, 12, 14, 12, 12, 10, 14, 12, 12, 10, 12, 10, 10,  8,
    14, 12, 12, 10, 12, 10, 10,  8, 12, 10, 10,  8, 10,  8,  8,  4,
    14, 12, 12, 10, 12, 10, 10,  8, 12, 10, 10,  8, 10,  8,  8,  6,
    12, 10, 10,  8, 10,  8,  8,  6, 10,  8,  8,  6,  8,  6,  6,  4,
  },
  {
    21, 19, 19, 17, 19, 17, 17, 15, 19, 17, 17, 15, 17, 15, 15, 13,
    19, 17, 17, 15, 17, 15, 15, 13, 17, 15, 15, 13, 15, 13, 13, 11,
    19, 17, 17, 15, 17, 15, 15, 13, 17, 15, 15, 13, 15, 13, 13, 11,
    17, 15, 15, 13, 15, 13, 13, 11, 15, 13, 13, 11, 13, 11, 11,  5,
    19, 17, 17, 15, 17, 15, 15, 13, 17, 15, 15, 13, 15, 13, 13, 11,
    17, 15, 15, 13, 15, 13, 13, 11, 15, 13, 13, 11, 13, 11, 11,  9,
    17, 15, 15, 13, 15, 13, 13, 11, 15, 13, 13, 11, 13, 11, 11,  9,
    15, 13, 13, 11, 13, 11, 11,  9, 13, 11, 11,  9, 11,  9,  9,  5,
    19, 17, 17, 15, 17, 15, 15, 13, 17, 15, 15, 13, 15, 13, 13, 11,
    17, 15, 15, 13, 15, 13, 13, 11, 15, 13, 13, 11, 13, 11, 11,  9,
    17, 15, 15, 13, 15, 13, 13, 11, 15, 13, 13, 11, 13, 11, 11,  9,
    15, 13, 13, 11, 13, 11, 11,  9, 13, 11, 11,  9, 11,  9,  9,  5,
    17, 15, 15, 13, 15, 13, 13, 11, 15, 13, 13, 11, 13, 11, 11,  9,
    15, 13, 13, 11, 13, 11, 11,  9, 13, 11, 11,  9, 11,  9,  9,  7,
    15, 13, 13, 11, 13, 11, 11,  9, 13, 11, 11,  9, 11,  9,  9,  7,
    13, 11, 11,  9, 11,  9,  9,  7, 11,  9,  9,  7,  9,  7,  7,  5,
  },
  {
    22, 20, 20, 18, 20, 18, 18, 16, 20, 18, 18, 16, 18, 16, 16, 14,
    20, 18, 18, 16, 18, 16, 16, 14, 18, 16, 16, 14, 16, 14, 14, 12,
    20, 18, 18, 16, 18, 16, 16, 14, 18, 16, 16, 14, 16, 14, 14, 12,
    18, 16, 16, 14, 16, 14, 14, 12, 16, 14, 14, 12, 14, 12, 12, 10,
    20, 18, 18, 16, 18, 16, 16, 14, 18, 16, 16, 14, 16, 14, 14, 12,
    18, 16, 16, 14, 16, 14, 14, 12, 16, 14, 14, 12, 14, 12, 12, 10,
    18, 16, 16, 14, 16, 14, 14, 12, 16, 14, 14, 12, 14, 12, 12, 10,
    16, 14, 14, 12, 14, 12, 12, 10, 14, 12, 12, 10, 12, 10, 10,  6,
    20, 18, 18, 16, 18, 16, 16, 14, 18, 16, 16, 14, 16, 14, 14, 12,
    18, 16, 16, 14, 16, 14, 14, 12, 16, 14, 14, 12, 14, 12, 12, 10,
    18, 16, 16, 14, 16, 14, 14, 12, 16, 14, 14, 12, 14, 12, 12, 10,
    16, 14, 14, 12, 14, 12, 12, 10, 14, 12, 12, 10, 12, 10, 10,  8,
    18, 16, 16, 14, 16, 14, 14, 12, 16, 14, 14, 12, 14, 12, 12, 10,
    16, 14, 14, 12, 14, 12, 12, 10, 14, 12, 12, 10, 12, 10, 10,  8,
    16, 14, 14, 12, 14, 12, 12, 10, 14, 12, 12, 10, 12, 10, 10,  8,
    14, 12, 12, 10, 12, 10, 10,  8, 12, 10, 10,  8, 10,  8,  8,  6,
  },
  {
    23, 21, 21, 19, 21, 19, 19, 17, 21, 19, 19, 17, 19, 17, 17, 15,
    21, 19, 19, 17, 19, 17, 17, 15, 19, 17, 17, 15, 17, 15, 15, 13,
    21, 19, 19, 17, 19, 17, 17, 15, 19, 17, 17, 15, 17, 15, 15, 13,
    19, 17, 17, 15, 17, 15, 15, 13, 17, 15, 15, 13, 15, 13, 13, 11,
    21, 19, 19, 17, 19, 17, 17, 15, 19, 17, 17, 15, 17, 15, 15, 13,
    19, 17, 17, 15, 17, 15, 15, 13, 17, 15, 15, 13, 15, 13, 13, 11,
    19, 17, 17, 15, 17, 15, 15, 13, 17, 15, 15, 13, 15, 13, 13, 11,
    17, 15, 15, 13, 15, 13, 13, 11, 15, 13, 13, 11, 13, 11, 11,  9,
    21, 19, 19, 17, 19, 17, 17, 15, 19, 17, 17, 15, 17, 15, 15, 13,
    19, 17, 17, 15, 17, 15, 15, 13, 17, 15, 15, 13, 15, 13, 13, 11,
    19, 17, 17, 15, 17, 15, 15, 13, 17, 15, 15, 13, 15, 13, 13, 11,
    17, 15, 15, 13, 15, 13, 13, 11, 15, 13, 13, 11, 13, 11, 11,  9,
    19, 17, 17, 15, 17, 15, 15, 13, 17, 15, 15, 13, 15, 13, 13, 11,
    17, 15, 15, 13, 15, 13, 13, 11, 15, 13, 13, 11, 13, 11, 11,  9,
    17, 15, 15, 13, 15, 13, 13, 11, 15, 13, 13, 11, 13, 11, 11,  9,
    15, 13, 13, 11, 13, 11, 11,  9, 13, 11, 11,  9, 11,  9,  9,  7,
  },
};


int32_t selectExcess(const ConstVector<uint8_t> &bytes, uint32_t id, int excess) {
  uint32_t byte_id = id / BIT_PER_BYTE;
  uint32_t offset = 0;
  int tmp_excess = excess;
  int i = 0;
  while (byte_id < bytes.count()) {
    ++i;
    if (i > 8)  return -1;

    uint8_t byte = bytes[byte_id];
    if (tmp_excess <= 8) {
      uint8_t value = _table[tmp_excess - 1][byte];
      if (value < 8)  return offset + value;

      tmp_excess = value - 7;
    } else {
      int excess_local = 8;
      tmp_excess -= excess_local;
      uint8_t value = _table[excess_local - 1][byte];
      tmp_excess += (value - 7);
    }
    ++byte_id;
    offset += 8;
  }
}


uint32_t countOne(uint8_t byte, int bit_count) {
  uint32_t count = 0;
  for (int i = 0; i < bit_count; ++i) {
    uint8_t mask = 1 << i;
    if (byte&mask)  ++count;
  }
  return count;
}

} // namespace

uint32_t ConstBalancedBitVector::_findCloseFar(uint32_t id) const {
  uint32_t is_far_id = rank0(id) - 1;
  uint32_t is_pioneer_id = _is_fars.rank1(is_far_id) - 1;
  int32_t pioneer_id = _is_pioneers.rank1(is_pioneer_id) - 1;
  if (pioneer_id < 0)  return count() - 1;

  uint32_t pioneer = _pioneers[(uint32_t)pioneer_id] * BIT_PER_UNIT;
  int32_t excess_left = this->excess(id) - 1;
  int excess = this->excess(pioneer - 1) - excess_left;
  uint32_t result = pioneer + selectExcess(bytes(), pioneer, excess);
  return result;
}

int32_t ConstBalancedBitVector::_findCloseNear(uint32_t id) const {
  uint32_t tmp_id = id;
  const ConstVector<uint8_t> &bytes = this->bytes();

  uint8_t bit_id = tmp_id % BIT_PER_BYTE;
  uint32_t byte_id = tmp_id / BIT_PER_BYTE;
  uint8_t byte = bytes[byte_id];
  uint8_t mask = ~(~0 << bit_id);
  byte |= mask;

  uint32_t result = 0;
  int32_t value = _table[bit_id][byte];
  if (value >= 8) {
    int excess = value - 7;
    tmp_id += 8;
    value = selectExcess(bytes, tmp_id, excess);
  } else if (value == -1) {
    return -1;
  }
  result = tmp_id + value - bit_id;
  return result;
}

uint32_t ConstBalancedBitVector::findClose(uint32_t id) const {
  assert(!bitAt(id));
  uint32_t result = 0;
  uint32_t is_far_id = rank0(id) - 1;
  if (_is_fars[is_far_id]) {
    result = _findCloseFar(id);
  } else {
    result = _findCloseNear(id + 1);
  }
  return result;
}

uint32_t ConstBalancedBitVector::findCloseNaive(uint32_t offset) const {
  int count = 0;
  for (int i = offset + 1; i < this->count(); i++) {
    if (operator[](i)) {
      if (count == 0)  return i;
      --count;
    } else {
      ++count;
    }
  }
  assert(false);
}

uint32_t ConstBalancedBitVector::findOpenNaive(uint32_t offset) const {
  assert(operator[](offset));
  int count = 0;
  for (int i = offset - 1; i >= 0; --i) {
    if (operator[](i)) {
      ++count;	
    } else {
      if (count == 0)  return i;
      --count;
    }
  }
  assert(false);
}

uint32_t ConstBalancedBitVector::excess(uint32_t id) const {
  uint32_t rank1 = this->rank1(id);
  uint32_t rank0 = id + 1 - rank1;
  return rank0 - rank1;
}

uint32_t ConstBalancedBitVector::encloseNaive(uint32_t id) const {
  assert(0 < id && id < count());
  assert(!bitAt(id));
  int count = 0;
  int i;
  for (i = id - 1; i >= 0; --i) {
    if (bitAt(i)) {
      --count;
    } else {
      if (count == 0)  break;
      ++count;
    }
  }
  assert(i >= 0);
  return i;
}

void ConstBalancedBitVector::read(istream &is) {
  ConstBitVector::read(is);
  _is_fars.read(is);
  _is_pioneers.read(is);
  _pioneers.read(is);
}

uint32_t ConstBalancedBitVector::mmap(const uint8_t* address) {
  const uint8_t *tmp_address = address;
  uint32_t consumed_size = ConstBitVector::mmap(tmp_address);
  tmp_address += consumed_size;

  consumed_size = _is_fars.mmap(tmp_address);
  tmp_address += consumed_size;

  consumed_size = _is_pioneers.mmap(tmp_address);
  tmp_address += consumed_size;

  consumed_size = _pioneers.mmap(tmp_address);
  tmp_address += consumed_size;

  return tmp_address - address;
}

void ConstBalancedBitVector::clear() {
  ConstBitVector::clear();
  _is_fars.clear();
  _is_pioneers.clear();
  _pioneers.clear();
}

void ConstBalancedBitVector::display(ostream &os) const {
  ConstBitVector::display(os);

  os << "pioneers: ";
  _pioneers.display();
}

void ConstBalancedBitVector::display() const {
  this->display(cout);
}

