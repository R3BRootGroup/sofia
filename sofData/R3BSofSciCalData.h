#ifndef R3BSOFSCICALITEM_H
#define R3BSOFSCICALITEM_H

#include "TObject.h"

// for the data analysis of the SOFIA SINGLE SCINTILLATORS detectors at S2 and Cave C
// Introduced by Audrey, April 2017

class R3BSofSciCalData : public TObject
{
  public:
    // Default Constructor
    R3BSofSciCalData();

    // Standard Constructor
    R3BSofSciCalData(UChar_t detector);

    // Destructor
    virtual ~R3BSofSciCalData();

    // Getters
    inline const UChar_t&  GetDetector()  const { return fDetector; }
    Double_t GetMeanTime();
    Double_t GetRawPos();

  private:
    UChar_t  fDetector;

  public:
    Double_t fTime_PMT1_ns;   // right
    Double_t fTime_PMT2_ns;   // left
    Double_t fTime_REF_ns;    // common time reference

    ClassDef(R3BSofSciCalData, 3)
};

#endif
