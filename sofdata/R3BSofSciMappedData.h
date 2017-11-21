#ifndef R3BSOFSCIMAPPEDITEM_H
#define R3BSOFSCIMAPPEDITEM_H

#include "TObject.h"

// for the data analysis of the SOFIA SINGLE SCINTILLATORS
// Introduced by Audrey, April 2017

class R3BSofSciMappedData : public TObject
{
  public:
    // Default Constructor
    R3BSofSciMappedData();

    // Standard Constructor
    R3BSofSciMappedData(UChar_t detector,
			UChar_t pmt,
			UInt_t  tc,
			UInt_t  tf
			);

    // Destructor
    virtual ~R3BSofSciMappedData() { }

    // Getters
    inline const UChar_t& GetDetector()   const {return fDetector;}
    inline const UChar_t& GetPmt()        const {return fPmt;}
    inline const UInt_t&  GetTimeCoarse() const {return fTimeCoarse;}
    inline const UInt_t&  GetTimeFine()   const {return fTimeFine;}

  private:
    UChar_t fDetector; // 1..n (2014, n=2, 1=S2, 2=Cave C)
    UChar_t fPmt;      // 1..2
    UInt_t  fTimeCoarse;
    UInt_t  fTimeFine;


  public:
    ClassDef(R3BSofSciMappedData, 2)
};

#endif
