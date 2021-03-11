#ifndef R3BSOFTWIMREADER_H
#define R3BSOFTWIMREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"

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
    void SetNumSections(Int_t num) { fSections = num; }
    void SetNumAnodes(Int_t num) { fAnodes = num; }
    void SetNumTref(Int_t num) { fTref = num; }
    void SetNumTtrig(Int_t num) { fTtrig = num; }

    /* Reader specific data structure from ucesb */
    EXT_STR_h101_SOFTWIM* fData;
    /* Data offset */
    UInt_t fOffset;
    // Don't store data for online
    Bool_t fOnline;
    /* the structs of type R3BSofTwimMappedData Item */
    TClonesArray* fArray; /**< Output array. */
    /* Geometry */
    Int_t fSections;
    Int_t fAnodes;
    Int_t fTref;
    Int_t fTtrig;

  public:
    ClassDef(R3BSofTwimReader, 0);
};

#endif
