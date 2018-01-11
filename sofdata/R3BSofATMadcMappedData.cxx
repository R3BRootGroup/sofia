#include "R3BSofATMadcMappedData.h"

// for the data analysis of the SOFIA - ACTIVE TARGET


R3BSofATMadcMappedData::R3BSofATMadcMappedData()
  : fAnode(0)
  , fEnergy(0)
{
}

R3BSofATMadcMappedData::R3BSofATMadcMappedData(UChar_t a,
					       UInt_t  e)
  : fAnode(a)
  , fEnergy(e)
{
}



ClassImp(R3BSofATMadcMappedData)
