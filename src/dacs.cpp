#include "dacs.h"
#include <assert.h>

#define FACT_RANK 20

typedef unsigned long ulong;

void display(uint *list, uint count) {
  for (int i = 0; i < count; ++i) {
    if (list[i])  printf("%d:%d\t", i, list[i]);
  }
  putchar('\n');
  putchar('\n');
}

ushort * optimizationk(uint * list,int listLength, int * nkvalues){
  const static int count = 64 + 1;
  uint count_int_bit[count] = { 0 }; // count of int whose bit count is i.
  for (int i = 0; i < listLength; ++i) {
    uint count_bit = bits(list[i]);
    ++count_int_bit[count_bit];
  }
  count_int_bit[1] += count_int_bit[0];
  count_int_bit[0] = 0;
  
  int bit_max = 0;
  for (int i = count - 1; i >= 0; --i) {
    if (count_int_bit[i]) {
      bit_max = i;
      break;
    }
  }
  assert(bit_max < 60);
  uint count_int[count] = { 0 };
  for (int i = 1; i < count; ++i) {
    count_int[i] = count_int[i - 1] + count_int_bit[i];
  }


  uint * fc = count_int;
  uint nBits = bit_max - 1;

  // 3 arrays. dynamic programming algorithm, see paper:
  // DACs: Bringing Direct Access to Variable-Length Codes.
  long * s = (long *) malloc(sizeof(long)*(nBits+1));
  uint * l = (uint *) malloc(sizeof(uint)*(nBits+1));
  uint * b = (uint *) malloc(sizeof(uint*)*(nBits+1));

  int m=nBits;
  s[m]=0;
  l[m]=0;
  b[m]=0;

  ulong currentSize;
  ulong minSize;
  uint minPos;
  for(int t = m; t >= 0; --t){
    minSize = -1;
    minPos = m;
    for(int i = m; i >= t+1; --i){
      currentSize = s[i]
        + (fc[m+1]-fc[t]) * (i-t+1)
        + (fc[m+1]-fc[t]) / FACT_RANK;
      if(minSize > currentSize) {
        minSize = currentSize;
        minPos=i;
      }
    }

    if(minSize < (fc[m+1]-fc[t]) * (m-t+1)){
      s[t]=minSize;
      l[t]=l[minPos]+1;
      b[t]=minPos-t;
    }
    else {
      s[t]=(fc[m+1]-fc[t])*(m-t+1);
      l[t]=1;
      b[t]=m-t+1;				
    }
  }

  // build the result array.
  int L = l[0];
  ushort *kvalues = (ushort*)malloc(sizeof(ushort)*L);

  int t=0;
  for(int k=0;k< L;k++){
    kvalues[k]=b[t];
    t = t+b[t];
  }

  free(l);
  free(b);
  free(s);

  (*nkvalues)=L;
  return kvalues;
}

FTRep* createFT(uint *list,uint listLength){
  FTRep * rep = (FTRep *) malloc(sizeof(struct sFTRep));
  rep->listLength = listLength;

  // compute kvalues array.
  int nkvalues;
  ushort* kvalues = optimizationk(list,listLength,&nkvalues);

  // fill tamtablebase ?
  ushort kval;
  uint oldval =0;
  uint newval =0;
  register uint i=0;
  uint multval=1;
  do {
    oldval=newval;
    if(i >= nkvalues) {
      kval = 1<<(kvalues[nkvalues-1]);
    } 
    else {
      kval = 1<<(kvalues[i]);	
    }
    multval*=kval;
    newval = oldval+multval;
    i++;
  } while(oldval < newval);
  rep->tamtablebase = i;

  // fill tablebase ?
  rep->tablebase = (uint *) malloc(sizeof(uint)*rep->tamtablebase);
  oldval =0;
  newval =0;
  multval=1;	
  for(uint i=0; i<rep->tamtablebase; i++){
    oldval=newval;
    if(i>=nkvalues){
      kval = 1<<(kvalues[nkvalues-1]);
    }
    else {
      kval=1<<(kvalues[i]);	
    }
    multval*=kval;
    newval = oldval+multval;
    rep->tablebase[i]=oldval;
  }	

  // 
  uint *levelSizeAux = (uint *) malloc(sizeof(uint)*rep->tamtablebase);
  for(uint i=0;i<rep->tamtablebase;i++){
    levelSizeAux[i]=0;
  }
  for (uint i=0;i<listLength;i++){
    uint value = list[i];
    for(int j=0;j<rep->tamtablebase;j++)
      if(value>=rep->tablebase[j])  levelSizeAux[j]++;
  }

  // compute nLevels
  int j=0;
  while((j<rep->tamtablebase)&&(levelSizeAux[j]!=0)){
    j++;
  }
  rep->nLevels = j;
  ///

  // fill the 2 array?
  rep->base = (uint *)malloc(sizeof(uint)*rep->nLevels);
  rep->base_bits = (ushort *)malloc(sizeof(ushort)*rep->nLevels);
  for(uint i=0;i<rep->nLevels;i++){
    if(i>=nkvalues){
      rep->base[i]=1<<(kvalues[nkvalues-1]);
      rep->base_bits[i]=kvalues[nkvalues-1];
    }
    else{
      rep->base[i]=1<<(kvalues[i]);
      rep->base_bits[i]=kvalues[i];
    }
  }

  // fill tabLevels?
  uint tamLevels =0;
  for(uint i=0;i<rep->nLevels;i++)
    tamLevels+=rep->base_bits[i]*levelSizeAux[i];
  rep->tamCode=tamLevels;

  // fill 3 array? 
  uint *cont = (uint *) malloc(sizeof(uint)*rep->tamtablebase);
  uint *contB = (uint *) malloc(sizeof(uint)*rep->tamtablebase);
  rep->iniLevel = (uint *)malloc(sizeof(uint)*rep->nLevels);		
  rep->levelsIndex = (uint *) malloc(sizeof(uint)*(rep->nLevels+1));
  rep->levelsIndex[0]=0;
  uint indexLevel=0;
  for(int j=0;j<rep->nLevels;j++){
    rep->levelsIndex[j+1]=rep->levelsIndex[j] + levelSizeAux[j];
    rep->iniLevel[j] = indexLevel;
    cont[j]=rep->iniLevel[j];
    indexLevel+=levelSizeAux[j]*rep->base_bits[j];
    contB[j]=rep->levelsIndex[j];
  }


  uint bits_BS_len = rep->levelsIndex[rep->nLevels-1]+1; 
  uint * bits_BS = (uint *) malloc(sizeof(uint)*(bits_BS_len/W+1));
  for(uint i=0; i<((bits_BS_len)/W+1);i++)
    bits_BS[i]=0;

  rep->levels = (uint *) malloc(sizeof(uint)*(tamLevels/W+1));
  for(uint i=0;i<listLength;i++){
    uint value = list[i];
    j=rep->nLevels-1;
    while(j>=0){
      if(value >= rep->tablebase[j]){
        uint newvalue = value- rep->tablebase[j];
        for(int k=0;k<j;k++){
          bitwrite(rep->levels,cont[k],rep->base_bits[k],newvalue%rep->base[k]);
          cont[k]+=rep->base_bits[k];
          contB[k]++;
          newvalue = newvalue/rep->base[k];
        }

        bitwrite(rep->levels,cont[j],rep->base_bits[j],newvalue%rep->base[j]);
        cont[j]+=rep->base_bits[j];
        contB[j]++;
        if(j<rep->nLevels-1){
          bitset(bits_BS,contB[j]-1);
        }
        break;
      }
      j--;
    }
  }

  bitset(bits_BS,bits_BS_len-1);
  rep->bS = createBitRankW32Int(bits_BS, bits_BS_len , 1, 20); 	

  // fill rankLevels;
  rep->rankLevels = (uint *) malloc(sizeof(uint)*rep->nLevels);
  for(int j=0;j<rep->nLevels;j++)
    rep->rankLevels[j]= rank(rep->bS, rep->levelsIndex[j]-1);

  free(cont);
  free(contB);
  free(levelSizeAux);
  free(kvalues);
  return rep;
}

// param is 1-based index.
uint accessFT(FTRep * listRep, uint param){
  uint ini = param - 1;
  uint cont = listRep->iniLevel[0] + ini * listRep->base_bits[0]; // offset of the chunk.
  uint *level = listRep->levels;
  uint readByte = bitread(level, cont, listRep->base_bits[0]);

  uint nLevels = listRep->nLevels;
  if(nLevels == 1)  return readByte;
  
  uint pos = listRep->levelsIndex[0] + ini;
  register uint j = 0; // chunk counter.
  uint mult=0; // bit index of this chunk in the whole integer.
  uint partialSum=0;
  while( !bitget(listRep->bS->data, pos) ){ // loop until bit 1, which means the last chunk.
    // sum.
    partialSum = partialSum + (readByte<<mult);
    mult += listRep->base_bits[j];

    // compute offset of chunk in next level.
    uint rankini = rank(listRep->bS, listRep->levelsIndex[j] + ini -1)
      - listRep->rankLevels[j];
    ini = ini - rankini;

    // read chunk in next level.
    ++j;
    cont = listRep->iniLevel[j] + ini * listRep->base_bits[j];
    readByte = bitread(level,cont,listRep->base_bits[j]);

    if(j == nLevels-1)  break;
    pos = listRep->levelsIndex[j] + ini;
  }
  partialSum = partialSum + (readByte << mult) + listRep->tablebase[j];
  return partialSum;
}

uint * decompressFT(FTRep * listRep, uint n){
  uint nLevels=listRep->nLevels;
  uint * cont = (uint *) malloc(sizeof(byte*)*listRep->nLevels);
  uint * pos = (uint *) malloc(sizeof(uint)*listRep->nLevels);
  for(uint j=0;j<nLevels;j++){
    cont[j]=listRep->iniLevel[j];
    pos[j]=listRep->levelsIndex[j];
  }

  uint * list = (uint *) malloc(sizeof(uint)*n);
  uint * level=listRep->levels;
  for(uint i=0;i<n;i++){
    uint j=0;
    uint readByte = bitread(level,cont[j],listRep->base_bits[j]);
    cont[j]+=listRep->base_bits[j];

    uint mult=0;
    uint partialSum=0;
    while((!bitget(listRep->bS->data,pos[j]))){
      pos[j]++;
      partialSum = partialSum+ (readByte<<mult);
      mult+=listRep->base_bits[j];
      j++;

      readByte = bitread(level,cont[j],listRep->base_bits[j]);
      cont[j]+=listRep->base_bits[j];

      if(j==nLevels-1)  break;
    }
    if(j<nLevels-1){
      pos[j]++;
    }

    partialSum = partialSum + (readByte<<mult) + listRep->tablebase[j];
    list[i]=partialSum;
  }
  free(cont);
  free(pos);

  return list;
}

void destroyFT(FTRep * rep){
  free(rep->levelsIndex);
  free(rep->iniLevel);
  free(rep->rankLevels);
  free(rep->tablebase);
  free(rep->levels);
  destroyBitRankW32Int(rep->bS);
  free(rep->base);
  free(rep->base_bits);
  free(rep);
}

void saveFT(FTRep * rep, char * filename){
  FILE * flist;
  flist = fopen(filename,"w");
  fwrite(&(rep->listLength),sizeof(uint),1,flist);
  fwrite(&(rep->nLevels),sizeof(byte),1,flist);
  fwrite(&(rep->tamCode),sizeof(uint),1,flist);
  fwrite(&(rep->tamtablebase),sizeof(uint),1,flist);
  fwrite(rep->tablebase,sizeof(uint),rep->tamtablebase,flist);	
  fwrite(rep->base_bits,sizeof(ushort),rep->nLevels,flist);
  fwrite(rep->base,sizeof(uint),rep->nLevels,flist);
  fwrite(rep->levelsIndex,sizeof(uint),rep->nLevels+1,flist);
  fwrite(rep->iniLevel,sizeof(uint),rep->nLevels,flist);
  fwrite(rep->rankLevels,sizeof(uint),rep->nLevels,flist);

  fwrite(rep->levels,sizeof(uint),(rep->tamCode/W+1),flist);

  save(rep->bS,flist);

  fclose(flist);	
}

FTRep* loadFT(char * filename){
  FILE * flist;
  FTRep * rep = (FTRep *) malloc(sizeof(struct sFTRep));
  flist = fopen(filename,"r");
  fread(&(rep->listLength),sizeof(uint),1,flist);
  fread(&(rep->nLevels),sizeof(byte),1,flist);
  fread(&(rep->tamCode),sizeof(uint),1,flist);

  fread(&(rep->tamtablebase),sizeof(uint),1,flist);
  rep->tablebase = (uint *) malloc(sizeof(uint)*rep->tamtablebase);
  fread(rep->tablebase,sizeof(uint),rep->tamtablebase,flist);	

  rep->base_bits = (ushort *) malloc(sizeof(ushort)*rep->nLevels);
  fread(rep->base_bits,sizeof(ushort),rep->nLevels,flist);

  rep->base = (uint *) malloc(sizeof(uint)*rep->nLevels);
  fread(rep->base,sizeof(uint),rep->nLevels,flist);

  rep->levelsIndex = (uint *) malloc(sizeof(uint)*(rep->nLevels+1));
  fread(rep->levelsIndex,sizeof(uint),rep->nLevels+1,flist);

  rep->iniLevel = (uint *) malloc(sizeof(uint)*rep->nLevels);
  fread(rep->iniLevel,sizeof(uint),rep->nLevels,flist);

  rep->rankLevels = (uint *) malloc(sizeof(uint)*(rep->nLevels));
  fread(rep->rankLevels,sizeof(uint),rep->nLevels,flist);

  rep->levels = (uint *) malloc(sizeof(uint)*(rep->tamCode/W+1));	
  fread(rep->levels,sizeof(uint),(rep->tamCode/W+1),flist);

  rep->bS = (bitRankW32Int *) malloc(sizeof(struct sbitRankW32Int));
  load(rep->bS,flist);	

  fclose(flist);

  return rep;
}

uint memoryUsage(FTRep* rep) {
  return sizeof(uint)*rep->tamtablebase 
    + sizeof(ushort)*rep->nLevels
    + sizeof(ushort)*rep->nLevels
    + sizeof(uint)*(rep->nLevels+1)
    + sizeof(uint)*(rep->nLevels)
    + sizeof(uint)*(rep->nLevels)
    + sizeof(uint)*(rep->tamCode/W+1)
    + spaceRequirementInBits(rep->bS)/8
    + sizeof(struct sFTRep);
}
