// -------------------------------------------------------------------------
// -----                      R3BSofTwimMappedData source file              -----
// -------------------------------------------------------------------------

#include "R3BSofTwimMappedData.h"

// -----   Default constructor   -------------------------------------------
R3BSofTwimMappedData::R3BSofTwimMappedData()
    : fSecID(0)
    , fAnodeID(0)
    , fTime(0)
    , fEnergy(0)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
R3BSofTwimMappedData::R3BSofTwimMappedData(Int_t secID, Int_t anodeID, Int_t time, Int_t energy)
    : fSecID(secID)
    , fAnodeID(anodeID)
    , fTime(time)
    , fEnergy(energy)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofTwimMappedData)
