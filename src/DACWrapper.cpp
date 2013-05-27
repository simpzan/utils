#include "DACWrapper.h"

using namespace std;

namespace {

template <typename T>
void writeValue(ostream &os, T value) {
  os.write((char *)&value, sizeof(value));
}

template <typename T>
void writeArray(ostream &os, T *values, int count) {
  os.write((char *)values, sizeof(T)*count);
}

template <typename T>
void readValue(istream &is, T &value) {
  is.read((char *)&value, sizeof(value));
}

template <typename T>
void readArray(istream &is, T *array, int count) {
  is.read((char *)array, sizeof(T) * count);
}

template <typename T>
void allocateReadArray(istream &is, T *&values, int count) {
  values = (T*)malloc(sizeof(T) * count);
  readArray(is, values, count);
}

}

void DACWrapper::write(ostream &os) {
  FTRep *dac = _dac;
  uint8_t nLevels = dac->nLevels;
  writeValue(os, dac->listLength);
  writeValue(os, nLevels);
  writeValue(os, dac->tamCode);
  writeValue(os, dac->tamtablebase);
  writeArray(os, dac->tablebase, dac->tamtablebase);
  writeArray(os, dac->base_bits, nLevels);
  writeArray(os, dac->base, nLevels);
  writeArray(os, dac->levelsIndex, nLevels+1);
  writeArray(os, dac->iniLevel, nLevels);
  writeArray(os, dac->rankLevels, nLevels);
  writeArray(os, dac->levels, dac->tamCode/W + 1);

  bitRankW32Int *bs = dac->bS;
  uint32_t n = bs->n;
  uint32_t s = bs->s;
  writeValue(os, n);
  writeValue(os, bs->factor);
  writeArray(os, bs->data, n/W + 1);
  writeArray(os, bs->Rs, n/s + 1);
}

void DACWrapper::read(istream &is) {
  FTRep *dac = (FTRep *)malloc(sizeof(FTRep));
  readValue(is, dac->listLength);
  readValue(is, dac->nLevels);
  readValue(is, dac->tamCode);
  readValue(is, dac->tamtablebase);

  allocateReadArray(is, dac->tablebase, dac->tamtablebase);
  allocateReadArray(is, dac->base_bits, dac->nLevels);
  allocateReadArray(is, dac->base, dac->nLevels);
  allocateReadArray(is, dac->levelsIndex, dac->nLevels + 1);
  allocateReadArray(is, dac->iniLevel, dac->nLevels);
  allocateReadArray(is, dac->rankLevels, dac->nLevels);
  allocateReadArray(is, dac->levels, dac->tamCode/W + 1);
  
  bitRankW32Int *bs = (bitRankW32Int *)malloc(sizeof(bitRankW32Int));
  readValue(is, bs->n);
  bs->b = 32;
  readValue(is, bs->factor);
  bs->s = bs->b * bs->factor;
  bs->integers = bs->n / W;
  allocateReadArray(is, bs->data, bs->n/W + 1);
  bs->owner = 1;
  allocateReadArray(is, bs->Rs, bs->n/bs->s + 1);

  dac->bS = bs;
  _dac = dac;
}

