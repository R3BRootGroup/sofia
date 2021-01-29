#ifndef R3BSOFATREADER_H
#define R3BSOFATREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"


struct EXT_STR_h101_SOFAT_t;
typedef struct EXT_STR_h101_SOFAT_t EXT_STR_h101_SOFAT;
typedef struct EXT_STR_h101_SOFAT_onion_t EXT_STR_h101_SOFAT_onion;

class FairLogger;

class R3BSofAtReader : public R3BReader
{
  public:
    R3BSofAtReader(EXT_STR_h101_SOFAT*, UInt_t);
    ~R3BSofAtReader();

  public:
    Bool_t Init(ext_data_struct_info*);
    Bool_t Read();
    void Reset();

    /** Accessor to select online mode **/
    void SetOnline(Bool_t option) { fOnline = option; }

  private:
    Bool_t ReadData(EXT_STR_h101_SOFAT_onion*);

  private:
    /* Reader specific data structure from ucesb */
    EXT_STR_h101_SOFAT* fData;
    /* Data offset */
    UInt_t fOffset;
    // Don't store data for online
    Bool_t fOnline;
    /* FairLogger */
    FairLogger* fLogger;
    /* the structs of type R3BSofAtMappedData Item */
    TClonesArray* fArray; /**< Output array. */

  public:
    ClassDef(R3BSofAtReader, 0);
};

#endif
