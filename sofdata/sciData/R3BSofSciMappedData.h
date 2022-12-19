#ifndef R3BSOFSCIMAPPEDITEM_H
#define R3BSOFSCIMAPPEDITEM_H

#include "TObject.h"

class R3BSofSciMappedData : public TObject
{
  public:
    // Default Constructor
    R3BSofSciMappedData();

    // Standard Constructor
    R3BSofSciMappedData(UShort_t detector, UShort_t pmt, UInt_t tc, UInt_t tf);

    // Destructor
    virtual ~R3BSofSciMappedData() {}

    // Getters
    inline const UShort_t& GetDetector() const { return fDetector; }
    inline const UShort_t& GetPmt() const { return fPmt; }
    inline const UInt_t& GetTimeCoarse() const { return fTimeCoarse; }
    inline const UInt_t& GetTimeFine() const { return fTimeFine; }

  private:
    UShort_t fDetector; // 1..n (2014, n=2, 1=S2, 2=Cave C)
    UShort_t fPmt;      // 1..3 (1: RIGHT, 2: LEFT, 3: REF)
    UInt_t fTimeCoarse;
    UInt_t fTimeFine;

  public:
    ClassDef(R3BSofSciMappedData, 2)
};

#endif
