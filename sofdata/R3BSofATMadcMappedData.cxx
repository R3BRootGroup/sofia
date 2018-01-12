#include "R3BSofATMadcMappedData.h"

// for the data analysis of the SOFIA - ACTIVE TARGET
// Introduced by Audrey in March 2017


R3BSofATMadcMappedData::R3BSofATMadcMappedData()
  : fEnergy(std::vector<UInt_t>(0))
{
}

R3BSofATMadcMappedData::R3BSofATMadcMappedData(	UInt_t nCh)
  : fEnergy(std::vector<UInt_t>(nCh))
{
}



ClassImp(R3BSofATMadcMappedData)
