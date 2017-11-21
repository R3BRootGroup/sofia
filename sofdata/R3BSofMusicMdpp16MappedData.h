#ifndef R3BSOFMUSICMDPP16MAPPEDITEM_H
#define R3BSOFMUSICMDPP16MAPPEDITEM_H

#include "TObject.h"

// for the data analysis of the SOFIA Twin-MUSIC or Triple-MUSIC readout by MDPP16
// Introduced by Audrey, April 2017

class R3BSofMusicMdpp16MappedData : public TObject
{
 public:
  // Default Constructor
  R3BSofMusicMdpp16MappedData();
  
  // Standard Constructor
  R3BSofMusicMdpp16MappedData(UChar_t plane,
			      UChar_t section,
			      UChar_t anode,
			      UInt_t  t,
			      UInt_t  e,
			      Bool_t  v
			      );
  
  // Destructor
  virtual ~R3BSofMusicMdpp16MappedData() { }
  
  // Getters
  inline const UChar_t& GetPlane()   const {return fPlane;}
  inline const UChar_t& GetSection() const {return fSection;}
  inline const UChar_t& GetAnode()   const {return fAnode;}
  inline const UInt_t&  GetTime()    const {return fTime;}
  inline const UInt_t&  GetEnergy()  const {return fEnergy;}
  inline const Bool_t&  IsValid()    const {return fValid;}
  
 private:
  UChar_t fPlane; 
  UChar_t fSection;      
  UChar_t fAnode;
  UInt_t  fTime;
  UInt_t  fEnergy;
  Bool_t  fValid;
  
  
 public:
  ClassDef(R3BSofMusicMdpp16MappedData, 2)
    };

#endif
