#include "R3BSofToFWMappedData.h"

// for the data analysis of the SOFIA TIME-OF-FLIGHT WALL
// Introduced by Audrey, April 2017


R3BSofToFWMappedData::R3BSofToFWMappedData()
  : fDetector(0)
  , fPmt(0)
  , fTimeCoarse(0)
  , fTimeFine(0)
  , fEnergy(0)
  , fValid(kTRUE)
{
}

R3BSofToFWMappedData::R3BSofToFWMappedData(UChar_t detector,
					   UChar_t pmt,
					   UInt_t tc,
					   UInt_t tf,
					   UInt_t e,
					   Bool_t v)
    : fDetector(detector)
    , fPmt(pmt)
    , fTimeCoarse(tc)
    , fTimeFine(tf)
    , fEnergy(e)
    , fValid(v)
{
}

ClassImp(R3BSofToFWMappedData)
