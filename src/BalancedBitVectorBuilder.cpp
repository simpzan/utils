#include "BalancedBitVectorBuilder.h"

#include <map>
#include <cassert>

using namespace std;

namespace {

void buildFars(const map<int, int> &closeMap,
		BitVectorBuilder &is_fars, vector<int> &fars) {
	for (map<int, int>::const_iterator itr = closeMap.begin();
			itr != closeMap.end();
			++itr) {
		int block_open = itr->first;
		int block_close = itr->second;
		bool is_far = block_close / BIT_PER_UNIT != block_open / BIT_PER_UNIT;
		is_fars.append(is_far);
		if (is_far) {
			fars.push_back(block_close);
		} 	
	}
}

void buildPioneers(const vector<int> &fars,
		BitVectorBuilder &is_pioneers, Vector<uint32_t> &pioneers) {
	int block_id_last = -1;
	for (int i = 0; i < fars.size(); ++i) {
		int block_id_this = fars[i];
		bool is_pioneer = block_id_this / BIT_PER_UNIT !=
		   	block_id_last / BIT_PER_UNIT;
		is_pioneers.append(is_pioneer);
		if (is_pioneer) {
			pioneers.append(block_id_this / BIT_PER_UNIT);
		}
		block_id_last = block_id_this;
	}
}

void buildCloseMap(const vector<bool> &bits, map<int, int> &closeMap) {
	vector<int> stack;
	for (int i = 0; i < bits.size(); ++i) {
		if (bits[i]) {
			assert(!stack.empty());
			int id_open = stack.back();
			closeMap[id_open] = i;
			stack.pop_back();
		} else {
			stack.push_back(i);
		}
	}
	assert(stack.empty());
}

void buildFindCloseIndex(const vector<bool> &bits,
		BitVectorBuilder &is_fars, BitVectorBuilder &is_pioneers, 
		Vector<uint32_t> &pioneers) {
	// build close map for each open parenthesis.
	map<int, int> closeMap;
	buildCloseMap(bits, closeMap);
	vector<int> fars;
	buildFars(closeMap, is_fars, fars);
	buildPioneers(fars, is_pioneers, pioneers);
}

} // namespace

void BalancedBitVectorBuilder::write(ostream &os) {
	buildFindCloseIndex(bits(), _is_fars, _is_pioneers, _pioneers);

	BitVectorBuilder::write(os);
	_is_fars.write(os);
	_is_pioneers.write(os);
	_pioneers.write(os);
}

void BalancedBitVectorBuilder::clear() {
	BitVectorBuilder::clear();
	_is_fars.clear();
	_is_pioneers.clear();
	_pioneers.clear();
}

uint64_t BalancedBitVectorBuilder::sizeWithBitcount(uint64_t count) {
  uint64_t size_parent = BitVectorBuilder::sizeWithBitcount(count);

  uint64_t count_node = count / 2;
  uint64_t size_is_fars = BitVectorBuilder::sizeWithBitcount(count_node);

  uint32_t count_far = count_node / 2;
  uint32_t size_is_pioneers = BitVectorBuilder::sizeWithBitcount(count_far);

  uint32_t count_pioneer = count_far / BIT_PER_UNIT;
  uint32_t size_pioneers = Vector<uint32_t>::sizeWithCount(count_pioneer);

  return size_parent + size_is_fars + size_is_pioneers + size_pioneers;
}
