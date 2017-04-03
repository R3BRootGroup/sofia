#ifndef R3BSOFATMADCREADER_H
#define R3BSOFATMADCREADER_H

#include "R3BReader.h"

struct EXT_STR_h101_SOFAT_MADC_t;
typedef struct EXT_STR_h101_SOFAT_MADC_t EXT_STR_h101_SOFAT_MADC;

class FairLogger;

class R3BSofATMadcReader : public R3BReader
{
 public:
  R3BSofATMadcReader(EXT_STR_h101_SOFAT_MADC *, UInt_t);
  ~R3BSofATMadcReader();
  
  Bool_t Init(ext_data_struct_info *);
  Bool_t Read();
  void Reset();
  
 private:
  /* Reader specific data structure from ucesb */
  EXT_STR_h101_SOFAT_MADC* fData;
  /* Data offset */
  UInt_t fOffset;
  /* FairLogger */
  FairLogger*	fLogger;
  /* the structs of type R3BSofATMadcMappedItem */
  TClonesArray* fArray; /**< Output array. */
  
 public:
  ClassDef(R3BSofATMadcReader, 0);
};

#endif

