#ifndef R3BSOFCOMREFREADER_H
#define R3BSOFCOMREFREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"

struct EXT_STR_h101_SOFCOMREF_t;
typedef struct EXT_STR_h101_SOFCOMREF_t EXT_STR_h101_SOFCOMREF;
class FairLogger;

class R3BSofComRefReader : public R3BReader
{
  public:
    R3BSofComRefReader(EXT_STR_h101_SOFCOMREF*, UInt_t);
    ~R3BSofComRefReader();

    Bool_t Init(ext_data_struct_info*);
    Bool_t Read();
    void Reset();

  private:
    /* Reader specific data structure from ucesb */
    EXT_STR_h101_SOFCOMREF* fData;
    /* Data offset */
    UInt_t fOffset;
    /* FairLogger */
    FairLogger* fLogger;
    /* the structs of type R3BSofComRefMapped Item */
    TClonesArray* fArray; /**< Output array. */

  public:
    ClassDef(R3BSofComRefReader, 0);
};

#endif
