#ifndef R3BSOFTOFWREADER_H
#define R3BSOFTOFWREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"

struct EXT_STR_h101_SOFTOFW_t;
typedef struct EXT_STR_h101_SOFTOFW_t EXT_STR_h101_SOFTOFW;
class FairLogger;

class R3BSofToFWReader : public R3BReader
{
  public:
    R3BSofToFWReader(EXT_STR_h101_SOFTOFW*, UInt_t);
    ~R3BSofToFWReader();

    Bool_t Init(ext_data_struct_info*);
    Bool_t Read();
    void Reset();

    /** Accessor to select online mode **/
    void SetOnline(Bool_t option) { fOnline = option; }

  private:
    /* Reader specific data structure from ucesb */
    EXT_STR_h101_SOFTOFW* fData;
    /* Data offset */
    UInt_t fOffset;
    // Don't store data for online
    Bool_t fOnline;
    /* the structs of type R3BSofToFWMapped Item */
    TClonesArray* fArray; /**< Output array. */

  public:
    ClassDef(R3BSofToFWReader, 0);
};

#endif
