#ifndef R3BSOFTRIMREADER_H
#define R3BSOFTRIMREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"

// section 1 : FIRST SECTION ALONG Z AXIS
// section 2 : MIDDLE SECTION ALONG Z AXIS
// section 3 : LAST SECTION ALONG Z AXIS

// anodes 1 to 6 : energy and time
// anode 7 : reference time
// anode 8 : trigger time

struct EXT_STR_h101_SOFTRIM_t;
typedef struct EXT_STR_h101_SOFTRIM_t EXT_STR_h101_SOFTRIM;
typedef struct EXT_STR_h101_SOFTRIM_onion_t EXT_STR_h101_SOFTRIM_onion;

class FairLogger;

class R3BSofTrimReader : public R3BReader
{
  public:
    R3BSofTrimReader(EXT_STR_h101_SOFTRIM*, UInt_t);
    R3BSofTrimReader(EXT_STR_h101_SOFTRIM*, UInt_t, Int_t);
    ~R3BSofTrimReader();

  public:
    Bool_t Init(ext_data_struct_info*);
    Bool_t Read();
    void Reset();

    /** Accessor to select online mode **/
    void SetOnline(Bool_t option) { fOnline = option; }

    void SetNumSections(Int_t num) { fSections = num; }

  private:
    Bool_t ReadData(EXT_STR_h101_SOFTRIM_onion*, UShort_t);

  private:
    /* Reader specific data structure from ucesb */
    EXT_STR_h101_SOFTRIM* fData;
    /* Data offset */
    UInt_t fOffset;
    // Don't store data for online
    Bool_t fOnline;
    /* FairLogger */
    FairLogger* fLogger;
    /* the structs of type R3BSofTrimMappedData Item */
    TClonesArray* fArray; /**< Output array. */
    /* Number of Sections */
    Int_t fSections;

  public:
    ClassDef(R3BSofTrimReader, 0);
};

#endif
