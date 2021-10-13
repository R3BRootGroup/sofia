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
    , fPileup(kFALSE)
    , fOverflow(kFALSE)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
R3BSofTwimMappedData::R3BSofTwimMappedData(Int_t secID, Int_t anodeID, Int_t time, Int_t energy, Bool_t pu, Bool_t ov)
    : fSecID(secID)
    , fAnodeID(anodeID)
    , fTime(time)
    , fEnergy(energy)
    , fPileup(pu)
    , fOverflow(ov)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofTwimMappedData)
