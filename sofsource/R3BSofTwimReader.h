#ifndef R3BSOFTWIMREADER_H
#define R3BSOFTWIMREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"

#define NUM_SOFTWIM_PLANES   2
#define NUM_SOFTWIM_SECTIONS 2
#define NUM_SOFTWIM_ANODES   16 
// channels : [0..15] = energies of anodes [1..16]
// channels : [16..31] = times of anodes [1..16]
// channel : 32 = reference time 

struct EXT_STR_h101_SOFTWIM_t;
typedef struct EXT_STR_h101_SOFTWIM_t EXT_STR_h101_SOFTWIM;
typedef struct EXT_STR_h101_SOFTWIM_onion_t EXT_STR_h101_SOFTWIM_onion;

class FairLogger;

class R3BSofTwimReader : public R3BReader
{
 public:
  R3BSofTwimReader(EXT_STR_h101_SOFTWIM *, UInt_t);
  ~R3BSofTwimReader();

 public:
  Bool_t Init(ext_data_struct_info *);
  Bool_t Read();
  void Reset();

 private:
  Bool_t ReadData(EXT_STR_h101_SOFTWIM_onion*, UShort_t, UShort_t);

 private:
  /* Reader specific data structure from ucesb */
  EXT_STR_h101_SOFTWIM* fData;
  /* Data offset */
  UInt_t fOffset;
  /* FairLogger */
  FairLogger*	fLogger;
  /* the structs of type R3BSofAnodeMapped Item */
  TClonesArray* fArrayAnodes; /**< Output array. */
  TClonesArray* fArrayTref; /**< Output array. */
  

 public:
  ClassDef(R3BSofTwimReader, 0);
};

#endif
