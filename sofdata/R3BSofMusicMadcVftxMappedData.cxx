#include "R3BSofMusicMadcVftxMappedData.h"

// for the data analysis of the SOFIA Twin-MUSIC or Triple-MUSIC
// in the case those MUSIC detectors are reaad out by MADC32 + VFTX modules


R3BSofMusicMadcVftxMappedData::R3BSofMusicMadcVftxMappedData()
  : fPlane(0)
  , fSection(0)
  , fAnode(0)
  , fTimeCoarse(0)
  , fTimeFine(0)
  , fEnergy(0)
  , fFlag(kFALSE)
{
}

R3BSofMusicMadcVftxMappedData::R3BSofMusicMadcVftxMappedData(UShort_t plane,
							     UShort_t section,
							     UShort_t anode,
							     UInt_t   tc,
							     UInt_t   tf,
							     UInt_t   e,
							     Bool_t   flag)
  : fPlane(plane)
  , fSection(section)
  , fAnode(anode)
  , fTimeCoarse(tc)
  , fTimeFine(tf)
  , fEnergy(e)
  , fFlag(flag)
{
}

ClassImp(R3BSofMusicMadcVftxMappedData)
