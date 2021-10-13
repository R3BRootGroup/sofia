#ifndef R3BSOFATREADER_H
#define R3BSOFATREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"
#include <Rtypes.h>

struct EXT_STR_h101_SOFAT_t;
typedef struct EXT_STR_h101_SOFAT_t EXT_STR_h101_SOFAT;
typedef struct EXT_STR_h101_SOFAT_onion_t EXT_STR_h101_SOFAT_onion;

class R3BSofAtReader : public R3BReader
{
  public:
    // Standard constructor
    R3BSofAtReader(EXT_STR_h101_SOFAT*, size_t);

    // Destructor
    virtual ~R3BSofAtReader();

    // Setup structure information
    virtual Bool_t Init(ext_data_struct_info*) override;

    // Read data from full event structure
    virtual Bool_t Read() override;

    // Reset
    virtual void Reset() override;

    // Accessor to select online mode
    void SetOnline(Bool_t option) { fOnline = option; }

  private:
    // Read data
    Bool_t ReadData(EXT_STR_h101_SOFAT_onion*);
    // Reader specific data structure from ucesb
    EXT_STR_h101_SOFAT* fData;
    // Data offset
    size_t fOffset;
    // Don't store data for online
    Bool_t fOnline;
    // R3BSofAtMappedData Item
    TClonesArray* fArray; /**< Output array. */

  public:
    ClassDefOverride(R3BSofAtReader, 0);
};

#endif // R3BSOFATREADER_H
