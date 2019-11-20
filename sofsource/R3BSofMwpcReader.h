#ifndef R3BSOFMWPCREADER_H
#define R3BSOFMWPCREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"

struct EXT_STR_h101_SOFMWPC_t;
typedef struct EXT_STR_h101_SOFMWPC_t EXT_STR_h101_SOFMWPC;
class FairLogger;

class R3BSofMwpcReader : public R3BReader
{
  public:
    R3BSofMwpcReader(EXT_STR_h101_SOFMWPC*, UInt_t);
    ~R3BSofMwpcReader();

    Bool_t Init(ext_data_struct_info*);
    Bool_t Read();
    void Reset();

  private:
    /* Reader specific data structure from ucesb */
    EXT_STR_h101_SOFMWPC* fData;
    /* Data offset */
    UInt_t fOffset;
    /* FairLogger */
    FairLogger* fLogger;
    /* the structs of type R3BSofMwpcMapped Item */
    TClonesArray* fArrayMwpc0; /**< Output array. */
    TClonesArray* fArrayMwpc1; /**< Output array. */
    TClonesArray* fArrayMwpc2; /**< Output array. */
    TClonesArray* fArrayMwpc3; /**< Output array. */
    UInt_t fNumEntries;

  public:
    ClassDef(R3BSofMwpcReader, 0);
};

#endif
