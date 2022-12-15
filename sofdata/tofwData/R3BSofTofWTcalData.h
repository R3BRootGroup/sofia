#ifndef R3BSOFTOFWTCALITEM_H
#define R3BSOFTOFWTCALITEM_H

#include "TObject.h"

class R3BSofTofWTcalData : public TObject
{
  public:
    // Default Constructor
    R3BSofTofWTcalData();

    // Standard Constructor
    R3BSofTofWTcalData(UShort_t detector, UShort_t pmt, Double_t t);

    // Destructor
    virtual ~R3BSofTofWTcalData() {}

    // Getters
    inline const UShort_t& GetDetector() const { return fDetector; }
    inline const UShort_t& GetPmt() const { return fPmt; }
    inline const Double_t& GetRawTimeNs() const { return fRawTimeNs; }

  private:
    UShort_t fDetector;   // 1..28
    UShort_t fPmt;        // 1..3
    Double_t fRawTimeNs;

  public:
    ClassDef(R3BSofTofWTcalData, 2)
};

#endif
