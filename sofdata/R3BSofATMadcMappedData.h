#ifndef R3BSOFATMADCMAPPEDITEM_H
#define R3BSOFATMADCMAPPEDITEM_H

#include "TObject.h"

// for the data analysis of the SOFIA - ACTIVE TARGET

class R3BSofATMadcMappedData : public TObject
{
  public:
    // Default Constructor
    R3BSofATMadcMappedData();

    // Standard Constructor - version under construction
    R3BSofATMadcMappedData(UShort_t a,
			   UInt_t   e);  

    // Destructor
    virtual ~R3BSofATMadcMappedData() { }

    // Getters - version under construction
    inline const UShort_t& GetAnode() const  { return fAnode; }
    inline const UInt_t&  GetEnergy() const { return fEnergy; }

  private:
    // version under construction
    UChar_t fAnode;    // 1..5
    UInt_t  fEnergy;   

  public:
    ClassDef(R3BSofATMadcMappedData, 2)
};

#endif
