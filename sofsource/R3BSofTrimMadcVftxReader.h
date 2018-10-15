#ifndef R3BSOFTRIMMADCVFTXREADER_H
#define R3BSOFTRIMMADCVFTXREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"

struct EXT_STR_h101_SOFTRIM_MADCVFTX_t;
typedef struct EXT_STR_h101_SOFTRIM_MADCVFTX_t EXT_STR_h101_SOFTRIM_MADCVFTX;
class FairLogger;

class R3BSofTrimMadcVftxReader : public R3BReader
{
 public:
  R3BSofTrimMadcVftxReader(EXT_STR_h101_SOFTRIM_MADCVFTX *, UInt_t);
  ~R3BSofTrimMadcVftxReader();

  Bool_t Init(ext_data_struct_info *);
  Bool_t Read();
  void Reset();

 private:
  /* Reader specific data structure from ucesb */
  EXT_STR_h101_SOFTRIM_MADCVFTX* fData;
  /* Data offset */
  UInt_t fOffset;
  /* FairLogger */
  FairLogger*	fLogger;
  /* the structs of type R3BSofMusicMadcVftxMapped Item */
  TClonesArray* fArray; /**< Output array. */


 public:
  ClassDef(R3BSofTrimMadcVftxReader, 0);
};

#endif
