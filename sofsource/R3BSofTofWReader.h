#ifndef R3BSOFTOFWREADER_H
#define R3BSOFTOFWREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"
#include <Rtypes.h>

struct EXT_STR_h101_SOFTOFW_t;
typedef struct EXT_STR_h101_SOFTOFW_t EXT_STR_h101_SOFTOFW;

class R3BSofTofWReader : public R3BReader
{
  public:
    // Standard constructor
    R3BSofTofWReader(EXT_STR_h101_SOFTOFW*, size_t);
    R3BSofTofWReader(EXT_STR_h101_SOFTOFW*, size_t, Int_t);

    // Destructor
    virtual ~R3BSofTofWReader();

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
    EXT_STR_h101_SOFTOFW* fData;
    // Data offset
    size_t fOffset;
    // Don't store data for online
    Bool_t fOnline;
    // R3BSofTofWMapped Item
    TClonesArray* fArray; /* Output array. */
    // Number of paddles
    Int_t fNumPaddles;

  public:
    ClassDefOverride(R3BSofTofWReader, 0);
};

#endif // R3BSOFTOFWREADER_H
