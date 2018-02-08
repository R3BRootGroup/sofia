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
    R3BSofComRefMappedData(UShort_t detector,
			   UInt_t   tc,
			   UInt_t   tf,
			   Bool_t   valid
			);

    // Destructor
    virtual ~R3BSofComRefMappedData() { }

    // Getters
    inline const UShort_t& GetDetector()   const {return fDetector;}
    inline const UInt_t&   GetTimeCoarse() const {return fTimeCoarse;}
    inline const UInt_t&   GetTimeFine()   const {return fTimeFine;}
    inline const Bool_t&   IsValid()       const {return fValid;}

  private:
    UShort_t fDetector; // 1..n (2014, n=2, 1=S2, 2=Cave C)
    UInt_t   fTimeCoarse;
    UInt_t   fTimeFine;
    Bool_t   fValid;
    // fValid is set to kTRUE only if mult is equal to one
    // otherwise it is impossible to reconstruct the incoming beam ToF


  public:
    ClassDef(R3BSofComRefMappedData, 2)
};

#endif
