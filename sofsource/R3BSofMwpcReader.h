#ifndef R3BSOFMWPCREADER_H
#define R3BSOFMWPCREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"
#include <Rtypes.h>

struct EXT_STR_h101_SOFMWPC_t;
typedef struct EXT_STR_h101_SOFMWPC_t EXT_STR_h101_SOFMWPC;

class R3BSofMwpcReader : public R3BReader
{
  public:
    // Standard constructor
    R3BSofMwpcReader(EXT_STR_h101_SOFMWPC*, size_t);

    // Destructor
    virtual ~R3BSofMwpcReader();

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
    EXT_STR_h101_SOFMWPC* fData;
    // Data offset
    size_t fOffset;
    // Don't store data for online
    Bool_t fOnline;
    // R3BSofMwpcMapped Item
    TClonesArray* fArrayMwpc0; /**< Output array. */
    TClonesArray* fArrayMwpc1; /**< Output array. */
    TClonesArray* fArrayMwpc2; /**< Output array. */
    TClonesArray* fArrayMwpc3; /**< Output array. */

  public:
    ClassDefOverride(R3BSofMwpcReader, 0);
};

#endif // R3BSOFMWPCREADER_H
