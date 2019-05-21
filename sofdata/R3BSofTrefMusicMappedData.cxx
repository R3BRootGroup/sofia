#include "R3BSofTrefMusicMappedData.h"


R3BSofTrefMusicMappedData::R3BSofTrefMusicMappedData()
  : fPlane(0)
  , fSection(0)
  , fTime(0)
  , fMultTref(0)
  , fnAnodes(0)
{
}

R3BSofTrefMusicMappedData::R3BSofTrefMusicMappedData(UShort_t plane,
						     UShort_t section,
						     UShort_t time,
						     UShort_t mult,
						     UShort_t nanodes)
    : fPlane(plane)
    , fSection(section)
    , fTime(time)
    , fMultTref(mult)
    , fnAnodes(nanodes)
{
}

ClassImp(R3BSofTrefMusicMappedData)
