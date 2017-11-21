#ifndef R3BSOFATMADCMAPPEDITEM_H
#define R3BSOFATMADCMAPPEDITEM_H

#include "TObject.h"
#include <vector>

// for the data analysis of the SOFIA - ACTIVE TARGET
// Introduced by Audrey, March 2017

class R3BSofATMadcMappedData : public TObject
{
  public:
    // Default Constructor
    R3BSofATMadcMappedData();

    // Standard Constructor
    R3BSofATMadcMappedData(UInt_t nChannels);

    // Destructor
    virtual ~R3BSofATMadcMappedData() { }

    // Getters
    inline const UInt_t&  GetEnergy(UInt_t channel) const { return fEnergy[channel]; }
    void SetEnergy(UInt_t channel,UInt_t energy)          {fEnergy[channel] = energy;};

  private:
    std::vector<UInt_t>  fEnergy;   

  public:
    ClassDef(R3BSofATMadcMappedData, 2)
};

#endif
