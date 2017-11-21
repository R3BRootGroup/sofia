#ifndef R3BSOFCOMREFMAPPEDITEM_H
#define R3BSOFCOMREFMAPPEDITEM_H

#include "TObject.h"

// for the data analysis of the COMMON REFERENCE TIME SIGNAL sent to S2 and Cave C
// Introduced by Audrey, April 2017

class R3BSofComRefMappedData : public TObject
{
  public:
    // Default Constructor
    R3BSofComRefMappedData();

    // Standard Constructor
    R3BSofComRefMappedData(UChar_t detector,
			UInt_t  tc,
			UInt_t  tf
			);

    // Destructor
    virtual ~R3BSofComRefMappedData() { }

    // Getters
    inline const UChar_t& GetDetector()   const {return fDetector;}
    inline const UInt_t&  GetTimeCoarse() const {return fTimeCoarse;}
    inline const UInt_t&  GetTimeFine()   const {return fTimeFine;}

  private:
    UChar_t fDetector; // 1..n (2014, n=2, 1=S2, 2=Cave C)
    UInt_t  fTimeCoarse;
    UInt_t  fTimeFine;


  public:
    ClassDef(R3BSofComRefMappedData, 2)
};

#endif
