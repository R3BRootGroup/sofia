#ifndef R3BSOFTRIMREADER_H
#define R3BSOFTRIMREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"
#include <Rtypes.h>

// section 1 : FIRST SECTION ALONG Z AXIS
// section 2 : MIDDLE SECTION ALONG Z AXIS
// section 3 : LAST SECTION ALONG Z AXIS

// anodes 1 to 6 : energy and time
// anode 7 : reference time
// anode 8 : trigger time

struct EXT_STR_h101_SOFTRIM_t;
typedef struct EXT_STR_h101_SOFTRIM_t EXT_STR_h101_SOFTRIM;
typedef struct EXT_STR_h101_SOFTRIM_onion_t EXT_STR_h101_SOFTRIM_onion;

class R3BSofTrimReader : public R3BReader
{
  public:
    // Standard constructor
    R3BSofTrimReader(EXT_STR_h101_SOFTRIM*, size_t);
    R3BSofTrimReader(EXT_STR_h101_SOFTRIM*, size_t, Int_t);

    // Destructor
    virtual ~R3BSofTrimReader();

    // Setup structure information
    virtual Bool_t Init(ext_data_struct_info*) override;

    // Read data from full event structure
    virtual Bool_t Read() override;

    // Reset
    virtual void Reset() override;

    // Accessor to select online mode
    void SetOnline(Bool_t option) { fOnline = option; }

    // Accessor to select the number of sections
    void SetNumSections(Int_t num) { fSections = num; }

  private:
    Bool_t ReadData(EXT_STR_h101_SOFTRIM_onion*, UShort_t);
    /* Reader specific data structure from ucesb */
    EXT_STR_h101_SOFTRIM* fData;
    /* Data offset */
    size_t fOffset;
    // Don't store data for online
    Bool_t fOnline;
    /* the structs of type R3BSofTrimMappedData Item */
    TClonesArray* fArray; /**< Output array. */
    /* Number of Sections */
    Int_t fSections;

  public:
    ClassDefOverride(R3BSofTrimReader, 0);
};

#endif // R3BSOFTRIMREADER_H
