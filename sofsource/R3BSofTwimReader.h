#ifndef R3BSOFTWIMREADER_H
#define R3BSOFTWIMREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"

#define NUM_SOFTWIM_SECTIONS 1
#define NUM_SOFTWIM_ANODES 20

// section 1 : RIGHT DOWN
// section 2 : RIGHT UP
// section 3 : LEFT DOWN
// section 4 : LEFT UP

// anodes 1 to 16 : energy and time
// anode 17 and 18 : reference time --> will be changed to 17 only when the full Twin-MUSIC will be cabled 
// anode 19 and 20 : trigger time   --> will be changed to 18 only when the full Twin-MUSIC will be cabled

struct EXT_STR_h101_SOFTWIM_t;
typedef struct EXT_STR_h101_SOFTWIM_t EXT_STR_h101_SOFTWIM;
typedef struct EXT_STR_h101_SOFTWIM_onion_t EXT_STR_h101_SOFTWIM_onion;

class R3BSofTwimReader : public R3BReader
{
  public:
    R3BSofTwimReader(EXT_STR_h101_SOFTWIM*, UInt_t);
    ~R3BSofTwimReader();

  public:
    Bool_t Init(ext_data_struct_info*);
    Bool_t Read();
    void Reset();

    /** Accessor to select online mode **/
    void SetOnline(Bool_t option) { fOnline = option; }

  private:
    Bool_t ReadData(EXT_STR_h101_SOFTWIM_onion*, UShort_t);

    uint32_t multPerAnode[NUM_SOFTWIM_ANODES];

    /* Reader specific data structure from ucesb */
    EXT_STR_h101_SOFTWIM* fData;
    /* Data offset */
    UInt_t fOffset;
    // Don't store data for online
    Bool_t fOnline;
    /* the structs of type R3BSofTwimMappedData Item */
    TClonesArray* fArray; /**< Output array. */

  public:
    ClassDef(R3BSofTwimReader, 0);
};

#endif
