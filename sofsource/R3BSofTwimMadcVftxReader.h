#ifndef R3BSOFTWIMMADCVFTXREADER_H
#define R3BSOFTWIMMADCVFTXREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"

struct EXT_STR_h101_SOFTWIM_MADCVFTX_t;
typedef struct EXT_STR_h101_SOFTWIM_MADCVFTX_t EXT_STR_h101_SOFTWIM_MADCVFTX;
class FairLogger;

class R3BSofTwimMadcVftxReader : public R3BReader
{
 public:
  R3BSofTwimMadcVftxReader(EXT_STR_h101_SOFTWIM_MADCVFTX *, UInt_t);
  ~R3BSofTwimMadcVftxReader();

  Bool_t Init(ext_data_struct_info *);
  Bool_t Read();
  void Reset();

 private:
  /* Reader specific data structure from ucesb */
  EXT_STR_h101_SOFTWIM_MADCVFTX* fData;
  /* Data offset */
  UInt_t fOffset;
  /* FairLogger */
  FairLogger*	fLogger;
  /* the structs of type R3BSofMusicMadcVftxMapped Item */
  TClonesArray* fArray; /**< Output array. */


 public:
  ClassDef(R3BSofTwimMadcVftxReader, 0);
};

#endif
