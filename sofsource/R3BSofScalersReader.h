#ifndef R3BSOFSCALERSREADER_H
#define R3BSOFSCALERSREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"

#include <Rtypes.h>

struct EXT_STR_h101_SOFSCALERS_t;
typedef struct EXT_STR_h101_SOFSCALERS_t EXT_STR_h101_SOFSCALERS;

class R3BSofScalersReader : public R3BReader
{
  public:
    // Standard constructor
    R3BSofScalersReader(EXT_STR_h101_SOFSCALERS*, size_t);

    // Destructor
    virtual ~R3BSofScalersReader();

    // Setup structure information
    virtual Bool_t Init(ext_data_struct_info*) override;

    // Read data from full event structure
    virtual Bool_t R3BRead() override;

    // Reset
    virtual void Reset() override;

    // Accessor to select online mode
    void SetOnline(Bool_t option) { fOnline = option; }

  private:
    // Reader specific data structure from ucesb
    EXT_STR_h101_SOFSCALERS* fData;
    // Data offset
    size_t fOffset;
    // Don't store data for online
    Bool_t fOnline;
    // R3BSofSciMapped Item
    TClonesArray* fArray; /* Output array. */
    UInt_t fNumEntries;

  public:
    ClassDefOverride(R3BSofScalersReader, 0);
};

#endif // R3BSOFSCALERSREADER_H
