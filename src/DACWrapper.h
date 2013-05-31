#ifndef DAC_WRAPPER_H
#define DAC_WRAPPER_H

#include <stdint.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include "dacs.h"

class DACWrapper {
  public:
  DACWrapper() : _dac(NULL) {}
  DACWrapper(uint32_t *array, int count) : _dac(NULL) {
    if (count && array)  _dac = createFT(array, count);
  }
  ~DACWrapper();

  void write(std::ostream &os);
  void read(std::istream &is);
  void clear();

  uint32_t operator[](int index);

  private:
  FTRep *_dac;
};

inline DACWrapper::~DACWrapper() {
  if (_dac)  destroyFT(_dac);
}

inline uint32_t DACWrapper::operator[](int index) {
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
