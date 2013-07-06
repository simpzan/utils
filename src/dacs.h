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
    uint listLength;  // integer count
    byte nLevels;     // chunk count ?

    uint * iniLevel;  // the bit offset of each level of chunks.
    ushort * base_bits;  // bit count in each level ofchunk
    uint * levels;    // levels of chunk concatenated.
    
    uint * levelsIndex; // offset of each level of bitmap.
    uint * rankLevels;

    uint * tablebase;
    bitRankW32Int * bS;	

    uint tamCode;     // 
    uint tamtablebase;
    uint * base;
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
