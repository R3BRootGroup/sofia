#include "R3BSofAnodeMappedData.h"

// for the data analysis of the SOFIA Twin-MUSIC or Triple-MUSIC
// in the case those MUSIC detectors are reaad out by MDPP16


R3BSofAnodeMappedData::R3BSofAnodeMappedData()
  : fPlane(0)
  , fSection(0)
  , fAnode(0)
  , fEnergy(0)
  , fTime(0)
{
}

R3BSofAnodeMappedData::R3BSofAnodeMappedData(UShort_t plane,
					     UShort_t section,
					     UShort_t anode)
  : fPlane(plane)
  , fSection(section)
  , fAnode(anode)
  , fEnergy(0)
  , fTime(0)
{
}

ClassImp(R3BSofAnodeMappedData)
