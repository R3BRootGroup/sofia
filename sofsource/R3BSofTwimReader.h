#ifndef R3BSOFTWIMREADER_H
#define R3BSOFTWIMREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"

#define NUM_SOFTWIM_SECTIONS 4
#define NUM_SOFTWIM_ANODES 17

// section 1 : RIGHT DOWN
// section 2 : RIGHT UP
// section 3 : LEFT DOWN
// section 4 : LEFT UP

// anodes 1 to 16 : energy and time
// anode 17 : reference time

struct EXT_STR_h101_SOFTWIM_t;
typedef struct EXT_STR_h101_SOFTWIM_t EXT_STR_h101_SOFTWIM;
typedef struct EXT_STR_h101_SOFTWIM_onion_t EXT_STR_h101_SOFTWIM_onion;

class FairLogger;

class R3BSofTwimReader : public R3BReader
{
  public:
    R3BSofTwimReader(EXT_STR_h101_SOFTWIM*, UInt_t);
    ~R3BSofTwimReader();

  public:
    Bool_t Init(ext_data_struct_info*);
    Bool_t Read();
    void Reset();

    uint32_t multPerAnode[NUM_SOFTWIM_ANODES];

  private:
    Bool_t ReadData(EXT_STR_h101_SOFTWIM_onion*, UShort_t);

  private:
    /* Reader specific data structure from ucesb */
    EXT_STR_h101_SOFTWIM* fData;
    /* Data offset */
    UInt_t fOffset;
    /* FairLogger */
    FairLogger* fLogger;
    /* the structs of type R3BSofTwimMappedData Item */
    TClonesArray* fArray; /**< Output array. */

  public:
    ClassDef(R3BSofTwimReader, 0);
};

#endif
