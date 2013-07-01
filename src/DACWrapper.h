#ifndef DAC_WRAPPER_H
#define DAC_WRAPPER_H

#include <stdint.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include "dacs.h"


class DACWrapper {
  public:
  DACWrapper() : _dac(NULL) {}
  DACWrapper(uint32_t *array, int count) : _dac(NULL) {
    if (count && array)  _dac = createFT(array, count);
  }
  DACWrapper(const std::vector<uint32_t> &v) {
    int size = v.size();
    if (size== 0)  return;
    _dac = createFT((uint32_t *)v.data(), size);
  }
  ~DACWrapper();

  void setData(uint32_t *array, int count) {
    clear();
    if (array && count)  _dac = createFT(array, count);
  }
  void write(std::ostream &os);
  void read(std::istream &is);
  void display(std::ostream &os) const;

  void clear();
  void swap(DACWrapper &rhs) {
    FTRep *dac = _dac;
    _dac = rhs._dac;
    rhs._dac = dac;
  }

  uint32_t operator[](int index) const;
  uint32_t count() const {
    return _dac->listLength;
  }

  private:
  FTRep *_dac;
};

inline DACWrapper::~DACWrapper() {
  if (_dac)  destroyFT(_dac);
}

inline uint32_t DACWrapper::operator[](int index) const {
  assert(_dac);
  return accessFT(_dac, index + 1);
}

inline void DACWrapper::clear() {
  if(_dac) {
    destroyFT(_dac);
    _dac = NULL;
  }
}

#endif
