#ifndef R3BSOFSCALERSMAPPEDITEM_H
#define R3BSOFSCALERSMAPPEDITEM_H

#include "TObject.h"

class R3BSofScalersMappedData : public TObject
{
  public:
    // Default Constructor
    R3BSofScalersMappedData();

    // Standard Constructor
    R3BSofScalersMappedData(UShort_t scaler, UShort_t channel, UInt_t value);

    // Destructor
    virtual ~R3BSofScalersMappedData() {}

    // Getters
    inline const UShort_t& GetScaler()  const {return fScaler;}
    inline const UShort_t& GetChannel() const {return fChannel;}
    inline const UInt_t&   GetValue()   const {return fValue;}

  private:
    UShort_t fScaler;
    UShort_t fChannel;
    UInt_t fValue;

  public:
    ClassDef(R3BSofScalersMappedData, 2)
};

#endif
