#ifndef R3BSOFCORRVREADER_H
#define R3BSOFCORRVREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"

struct EXT_STR_h101_SOFCORRV_t;
typedef struct EXT_STR_h101_SOFCORRV_t EXT_STR_h101_SOFCORRV;
class FairLogger;

class R3BSofCorrvReader : public R3BReader
{
  public:
    R3BSofCorrvReader(EXT_STR_h101_SOFCORRV*, UInt_t);
    ~R3BSofCorrvReader();

    Bool_t Init(ext_data_struct_info*);
    Bool_t Read();
    void Reset();

    /** Accessor to select online mode **/
    void SetOnline(Bool_t option) { fOnline = option; }

  private:
    /* Reader specific data structure from ucesb */
    EXT_STR_h101_SOFCORRV* fData;
    /* Data offset */
    UInt_t fOffset;
    // Don't store data for online
    Bool_t fOnline;
    /* the structs of type R3BSofCorrvMapped Item */
    TClonesArray* fArray; /**< Output array. */

  public:
    ClassDef(R3BSofCorrvReader, 0);
};

#endif
