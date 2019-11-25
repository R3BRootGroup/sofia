#ifndef R3BSOFSCITCALITEM_H
#define R3BSOFSCITCALITEM_H

#include "TObject.h"

class R3BSofSciTcalData : public TObject
{
  public:
    // Default Constructor
    R3BSofSciTcalData();

    // Standard Constructor
    R3BSofSciTcalData(UShort_t detector, UShort_t pmt, Double_t t);

    // Destructor
    virtual ~R3BSofSciTcalData() {}

    // Getters
    inline const UShort_t& GetDetector() const { return fDetector; }
    inline const UShort_t& GetPmt() const { return fPmt; }
    inline const Double_t& GetRawTimeNs() const { return fRawTimeNs; }

  private:
    UShort_t fDetector; // 1..n (2014, n=2, 1=S2, 2=Cave C)
    UShort_t fPmt;      // 1..3
    Double_t fRawTimeNs;

  public:
    ClassDef(R3BSofSciTcalData, 2)
};

#endif
