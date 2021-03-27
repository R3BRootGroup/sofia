#ifndef R3BSOFCORRMREADER_H
#define R3BSOFCORRMREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"

struct EXT_STR_h101_SOFCORRM_t;
typedef struct EXT_STR_h101_SOFCORRM_t EXT_STR_h101_SOFCORRM;
typedef struct EXT_STR_h101_SOFCORRM_onion_t EXT_STR_h101_SOFCORRM_onion;

class FairLogger;

class R3BSofCorrmReader : public R3BReader
{
  public:
    R3BSofCorrmReader(EXT_STR_h101_SOFCORRM*, UInt_t);
    ~R3BSofCorrmReader();

  public:
    Bool_t Init(ext_data_struct_info*);
    Bool_t Read();
    void Reset();

    /** Accessor to select online mode **/
    void SetOnline(Bool_t option) { fOnline = option; }

  private:
    /* Reader specific data structure from ucesb */
    EXT_STR_h101_SOFCORRM* fData;
    /* Data offset */
    UInt_t fOffset;
    // Don't store data for online
    Bool_t fOnline;
    /* FairLogger */
    FairLogger* fLogger;
    /* the structs of type R3BSofCorrmMappedData Item */
    TClonesArray* fArray; /**< Output array. */

  public:
    ClassDef(R3BSofCorrmReader, 0);
};

#endif
