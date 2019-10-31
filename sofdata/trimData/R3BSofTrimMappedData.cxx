// -------------------------------------------------------------------------
// -----                      R3BSofTrimMappedData source file              -----
// -------------------------------------------------------------------------

#include "R3BSofTrimMappedData.h"

// -----   Default constructor   -------------------------------------------
R3BSofTrimMappedData::R3BSofTrimMappedData()
    : fSecID(0)
    , fAnodeID(0)
    , fTime(0)
    , fEnergy(0)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
R3BSofTrimMappedData::R3BSofTrimMappedData(UChar_t secID, UChar_t anodeID, UShort_t time, UShort_t energy)
    : fSecID(secID)
    , fAnodeID(anodeID)
    , fTime(time)
    , fEnergy(energy)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofTrimMappedData)
