
#ifndef R3BSofWhiterabbitReader_H
#define R3BSofWhiterabbitReader_H

#include "R3BReader.h"
#include <Rtypes.h>

struct EXT_STR_h101_WRSOFIA_t;
typedef struct EXT_STR_h101_WRSOFIA_t EXT_STR_h101_WRSOFIA;

class TClonesArray;
class R3BEventHeader;

class R3BSofWhiterabbitReader : public R3BReader
{
  public:
    // Standard constructor
    R3BSofWhiterabbitReader(EXT_STR_h101_WRSOFIA*, size_t, UInt_t, UInt_t);

    // Destructor
    virtual ~R3BSofWhiterabbitReader();

    // Setup structure information
    virtual Bool_t Init(ext_data_struct_info*) override;

    // Read data from full event structure
    virtual Bool_t Read() override;

    // Reset
    virtual void Reset() override;

    /** Accessor to select online mode **/
    void SetOnline(Bool_t option) { fOnline = option; }

  private:
    /* An event counter */
    UInt_t fNEvent;
    /* Reader specific data structure from ucesb */
    EXT_STR_h101_WRSOFIA* fData;
    /* Offset of detector specific data in full data structure */
    size_t fOffset;
    /* The whiterabbit subsystem ID */
    UInt_t fWhiterabbitId1;
    UInt_t fWhiterabbitId2;
    /* A pointer to the R3BEventHeader structure */
    R3BEventHeader* fEventHeader;
    // Don't store data for online
    Bool_t fOnline;
    /**< Output array. */
    TClonesArray* fArray;

  public:
    ClassDefOverride(R3BSofWhiterabbitReader, 0);
};
#endif // R3BSofWhiterabbitReader_H
