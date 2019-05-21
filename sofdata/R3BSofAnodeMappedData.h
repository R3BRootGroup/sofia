#ifndef R3BSOFANODEMAPPEDITEM_H
#define R3BSOFANODEMAPPEDITEM_H

#include "TObject.h"


class R3BSofAnodeMappedData : public TObject
{
 public:
  // Default Constructor
  R3BSofAnodeMappedData();
  
  // Standard Constructor
  R3BSofAnodeMappedData(UShort_t plane,
			UShort_t section,
			UShort_t anode
			);
  
  // Destructor
  virtual ~R3BSofAnodeMappedData() { }
  
  // Getters
  inline const UShort_t& GetPlane()      const {return fPlane;}
  inline const UShort_t& GetSection()    const {return fSection;}
  inline const UShort_t& GetAnode()      const {return fAnode;}
  inline const UShort_t& GetEnergy()     const {return fEnergy;}
  inline const UShort_t& GetTime()       const {return fTime;}
  
 protected:
  UShort_t fPlane; 
  UShort_t fSection;      
  UShort_t fAnode;
 public:
  UShort_t fEnergy;   // DATA16
  UShort_t fTime;     // DATA16
  
  
 public:
  ClassDef(R3BSofAnodeMappedData, 2)
    };

#endif
