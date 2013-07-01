#ifndef DAC_H
#define DAC_H

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
//#include <malloc.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "basics.h"
#include "bitrankw32int.h"

  typedef struct sFTRep {
    uint listLength;
    byte nLevels;
    uint tamCode;
    uint * levels;
    uint * levelsIndex;
    uint * iniLevel;
    uint * rankLevels;
    bitRankW32Int * bS;	
    uint * base;
    ushort * base_bits;
    uint * tablebase;
    uint tamtablebase;
  } FTRep;



  // public:
  FTRep* createFT(uint *list,uint listLength);
  void destroyFT(FTRep * listRep);
  uint * decompressFT(FTRep * listRep, uint n);

  uint accessFT(FTRep * listRep,uint param);

  void saveFT(FTRep * listRep, char * filename);
  FTRep* loadFT(char * filename);

#ifdef __cplusplus
}
#endif

#endif
