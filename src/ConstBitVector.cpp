#include <cassert>

#include "ConstBitVector.h"
#include "BitVectorBuilder.h"

using namespace std;

namespace {

// count 1 bit in v.
// http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
template <typename T>
int popcount(T v) {
  v = v - ((v >> 1) & (T)~(T)0/3);                           // temp
  v = (v & (T)~(T)0/15*3) + ((v >> 2) & (T)~(T)0/15*3);      // temp
  v = (v + (v >> 4)) & (T)~(T)0/255*15;                      // temp
  int c = (T)(v * ((T)~(T)0/255)) >> (sizeof(T) - 1) * CHAR_BIT; // count
  return c;
}

uint32_t select(const ConstBitVector &bits, uint32_t count, bool bit) {
  uint32_t count_bit = bits.count();
  assert(count < count_bit);
  int tmpCount = 0;
  for (int ai = 0; ai < count_bit; ai++) {
    if (bits[ai] != bit) {
      continue;
    }
    ++tmpCount;
    if (count == tmpCount) {
      return ai;
    }
  }
  assert(false);
  return 0;
}

UnitT getPreviousUnit(const ConstVector<uint8_t> &bytes, uint32_t id) {
  UnitT unit = 0;
  uint8_t *p = (uint8_t *)&unit;

  uint32_t byte_id = id / BIT_PER_BYTE;
  uint32_t count_byte = byte_id % 8;
  uint32_t base = byte_id - count_byte;
  for (int i = 0; i < count_byte+1; ++i) {
    if (base + i >= bytes.size())  break;
    *p = bytes[base + i];
    ++p;
  }

  uint8_t bit_id = id % BIT_PER_BYTE;
  uint8_t shift = (8 - count_byte -1) * BIT_PER_BYTE + 
    (BIT_PER_BYTE - 1 - bit_id);
  UnitT mask = ~0ULL >> shift;
  unit &= mask;
  return unit;
}

} // namespace

bool ConstBitVector::operator[](uint32_t offset) const {
  assert(offset < _count_bit);
  uint32_t byte_id = offset / 8;
  uint8_t byte = _bytes[byte_id];
  uint32_t bit_id = offset % 8;
  return byte >> bit_id & 1;
}

uint32_t ConstBitVector::rank1(uint32_t offset) const {
  uint32_t block_id = offset / BIT_PER_BLOCK;
  uint32_t count = _ranks_block[block_id];	// block

  uint32_t unit_id = offset / BIT_PER_UNIT % UNIT_PER_BLOCK;
  if (unit_id != 0) {
    unit_id += block_id * 3; // there are 3 unit_index in a block.
    count += _ranks_unit[unit_id - 1];  // unit
  }

  UnitT unit = getPreviousUnit(_bytes, offset);
  count += popcount(unit);
  return count;
}

uint32_t ConstBitVector::rank1Naive(uint32_t offset) const {
  assert(offset < count());
  uint32_t count = 0;
  for (int i = 0; i <= offset; i++) {
    if (!bitAt(i)) {
      continue;
    }
    ++count;
  }
  return count;
}

uint32_t ConstBitVector::select1Naive(uint32_t count) const {
  return select(*this, count, true);
}

uint32_t ConstBitVector::select0Naive(uint32_t count) const {
  return select(*this, count, false);
}

void ConstBitVector::read(istream &is) {
  is.read((char *)&_count_bit, sizeof(uint32_t));

  _bytes.read(is);
  _ranks_block.read(is);
  _ranks_unit.read(is);
}

uint32_t ConstBitVector::mmap(const uint8_t *address) {
  const uint8_t *tmp_address = address;
  _count_bit = *(uint32_t *)tmp_address;
  tmp_address += sizeof(_count_bit);

  uint32_t consumed_size = _bytes.mmap(tmp_address);
  tmp_address += consumed_size;

  consumed_size = _ranks_block.mmap(tmp_address);
  tmp_address += consumed_size;

  consumed_size = _ranks_unit.mmap(tmp_address);
  tmp_address += consumed_size;

  return tmp_address - address;
}

void ConstBitVector::clear() {
  _count_bit = 0;
  _bytes.clear();
  _ranks_block.clear();
  _ranks_unit.clear();
}

void ConstBitVector::display(ostream &os) const {
  uint32_t count = this->count();
  os << "bit count: " << count << endl;
  for (int i = 0; i < count; ++i) {
    if (i % 4 == 0)  os << " ";
    if (i % 8 == 0)  os << " " << i <<":";
    os << (bitAt(i) ? 1 : 0);
  }
  os << endl;

  /* cout << "ranks_block: " << endl;*/
  //os << dec;
  //_ranks_block.display(os);
  //cout << "ranks_unit: " << endl;
  /*_ranks_unit.display(os);*/
}

