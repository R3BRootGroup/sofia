#ifndef R3BSOFSCALERSREADER_H
#define R3BSOFSCALERSREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"

struct EXT_STR_h101_SOFSCALERS_t;
typedef struct EXT_STR_h101_SOFSCALERS_t EXT_STR_h101_SOFSCALERS;
class FairLogger;

class R3BSofScalersReader : public R3BReader
{
  public:
    R3BSofScalersReader(EXT_STR_h101_SOFSCALERS*, UInt_t);
    ~R3BSofScalersReader();

    Bool_t Init(ext_data_struct_info*);
    Bool_t Read();
    void Reset();

    /** Accessor to select online mode **/
    void SetOnline(Bool_t option) { fOnline = option; }

  private:
    /* Reader specific data structure from ucesb */
    EXT_STR_h101_SOFSCALERS* fData;
    /* Data offset */
    UInt_t fOffset;
    // Don't store data for online
    Bool_t fOnline;
    /* the structs of type R3BSofSciMapped Item */
    TClonesArray* fArray; /**< Output array. */
    UInt_t fNumEntries;

  public:
    ClassDef(R3BSofScalersReader, 0);
};

#endif
