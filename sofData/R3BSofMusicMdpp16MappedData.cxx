#include "R3BSofMusicMdpp16MappedData.h"

// for the data analysis of the SOFIA Twin-MUSIC or Triple-MUSIC 
// in the case those MUSIC detectors are reaad out by MDPP16 modules
// Introduced by Audrey, April 2017


R3BSofMusicMdpp16MappedData::R3BSofMusicMdpp16MappedData()
  : fPlane(0)
  , fSection(0)
  , fAnode(0)
  , fTime(0)
  , fEnergy(0)
  , fValid(kTRUE)
{
}

R3BSofMusicMdpp16MappedData::R3BSofMusicMdpp16MappedData(UChar_t plane,
							 UChar_t section,
							 UChar_t anode,
							 UInt_t t,
							 UInt_t e,
							 UInt_t v)
  : fPlane(plane)
  , fSection(section)
  , fAnode(anode)
  , fTime(t)
  , fEnergy(e)
  , fValid(v)
{
}

ClassImp(R3BSofMusicMdpp16MappedData)
