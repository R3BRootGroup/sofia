#ifndef R3BSOFSCIREADER_H
#define R3BSOFSCIREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"
#include <Rtypes.h>

struct EXT_STR_h101_SOFSCI_t;
typedef struct EXT_STR_h101_SOFSCI_t EXT_STR_h101_SOFSCI;

class R3BSofSciReader : public R3BReader
{
  public:
    // Standard constructor
    R3BSofSciReader(EXT_STR_h101_SOFSCI*, size_t);
    R3BSofSciReader(EXT_STR_h101_SOFSCI*, size_t, Int_t);

    // Destructor
    virtual ~R3BSofSciReader();

    // Setup structure information
    virtual Bool_t Init(ext_data_struct_info*) override;

    // Read data from full event structure
    virtual Bool_t Read() override;

    // Reset
    virtual void Reset() override;

    // Accessor to select online mode
    void SetOnline(Bool_t option) { fOnline = option; }

  private:
    // Reader specific data structure from ucesb
    EXT_STR_h101_SOFSCI* fData;
    // Data offset
    size_t fOffset;
    // Don't store data for online
    Bool_t fOnline;
    // Output array of type R3BSofSciMapped
    TClonesArray* fArray;

    UInt_t fNumEntries;
    Int_t fNumSci;

  public:
    ClassDefOverride(R3BSofSciReader, 0);
};

#endif // R3BSOFSCIREADER_H
