#ifndef R3BSOFMUSICMADCVFTXMAPPEDITEM_H
#define R3BSOFMUSICMADCVFTXMAPPEDITEM_H

#include "TObject.h"

// for the data analysis of the SOFIA Twin-MUSIC or Triple-MUSIC readout by MADC32 + VFTX (as in October 2014)

class R3BSofMusicMadcVftxMappedData : public TObject
{
 public:
  // Default Constructor
  R3BSofMusicMadcVftxMappedData();
  
  // Standard Constructor
  R3BSofMusicMadcVftxMappedData(UShort_t plane,
				UShort_t section,
				UShort_t anode,
				UInt_t   tc,
				UInt_t   tf,
				UInt_t   e,
				Bool_t   flag
				);
  
  // Destructor
  virtual ~R3BSofMusicMadcVftxMappedData() { }
  
  // Getters
  inline const UShort_t& GetPlane()      const {return fPlane;}
  inline const UShort_t& GetSection()    const {return fSection;}
  inline const UShort_t& GetAnode()      const {return fAnode;}
  inline const UInt_t&   GetTimeCoarse() const {return fTimeCoarse;}
  inline const UInt_t&   GetTimeFine()   const {return fTimeFine;}
  inline const UInt_t&   GetEnergy()     const {return fEnergy;}
  inline const Bool_t&   WhichFlag()     const {return fFlag;}
  
 private:
  UShort_t fPlane; 
  UShort_t fSection;      
  UShort_t fAnode;
  UInt_t   fTimeCoarse;
  UInt_t   fTimeFine;
  UInt_t   fEnergy;
  Bool_t   fFlag;
  
  
 public:
  ClassDef(R3BSofMusicMadcVftxMappedData, 2)
    };

#endif
