#ifndef R3BSOFTOFWMAPPEDITEM_H
#define R3BSOFTOFWMAPPEDITEM_H

#include "TObject.h"

// for the data analysis of the SOFIA TIME-OF-FLIGHT WALL

class R3BSofToFWMappedData : public TObject
{
  public:
    // Default Constructor
    R3BSofToFWMappedData();

    // Standard Constructor
    R3BSofToFWMappedData(UShort_t detector, UShort_t pm, UInt_t tc, UInt_t tf, UInt_t e, Bool_t flag);

    // Destructor
    virtual ~R3BSofToFWMappedData() {}

    // Getters
    inline const UShort_t& GetDetector() const { return fDetector; }
    inline const UShort_t& GetPmt() const { return fPmt; }
    inline const UInt_t& GetTimeCoarse() const { return fTimeCoarse; }
    inline const UInt_t& GetTimeFine() const { return fTimeFine; }
    inline const UInt_t& GetEnergy() const { return fEnergy; }
    inline const Bool_t& WhichFlag() const { return fFlag; }

  private:
    UShort_t fDetector; // 1..28
    UShort_t fPmt;      // 1..2
    UInt_t fTimeCoarse;
    UInt_t fTimeFine;
    UInt_t fEnergy;
    Bool_t fFlag;
    // fFlag is set to kTRUE only if both PMTs fired once only
    // Any event which has at least one plastic turned to kFALSE should be discarded

  public:
    ClassDef(R3BSofToFWMappedData, 2)
};

#endif
