#ifndef R3BSOFTOFWMAPPEDITEM_H
#define R3BSOFTOFWMAPPEDITEM_H

#include "TObject.h"

// for the data analysis of the SOFIA TIME-OF-FLIGHT WALL
// Introduced by Audrey, April 2017

class R3BSofToFWMappedData : public TObject
{
  public:
    // Default Constructor
    R3BSofToFWMappedData();

    // Standard Constructor
    R3BSofToFWMappedData(UChar_t detector,
			 UChar_t pm,
			 UInt_t  tc,
			 UInt_t  tf,
			 UInt_t  e,
			 Bool_t  v
			);

    // Destructor
    virtual ~R3BSofToFWMappedData() { }

    // Getters
    inline const UChar_t& GetDetector()      const {return fDetector;}
    inline const UChar_t& GetPmt()           const {return fPmt;}
    inline const UInt_t&  GetTimeCoarse()    const {return fTimeCoarse;}
    inline const UInt_t&  GetTimeFine()      const {return fTimeFine;}
    inline const UInt_t&  GetEnergy()        const {return fEnergy;}
    inline const Bool_t&  IsValid()          const {return fValid;}

  private:
    UChar_t fDetector;    // 1..28
    UChar_t fPmt;         // 1..2
    UInt_t  fTimeCoarse;
    UInt_t  fTimeFine;
    UInt_t  fEnergy;
    Bool_t  fValid;       // if mult >1 on one plastic, event is not valid discarded


  public:
    ClassDef(R3BSofToFWMappedData, 2)
};

#endif
