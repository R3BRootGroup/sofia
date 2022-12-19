// -------------------------------------------------------------------------
// -----                     R3BSofSciHitData source file             -----
// -------------------------------------------------------------------------

#include "R3BSofSciHitData.h"

// -----   Default constructor   -------------------------------------------
R3BSofSciHitData::R3BSofSciHitData()
    : fSciId(0)
    , fX(0.)
    , fTime(0.)
{
}

// -----   Standard constructor   ------------------------------------------
R3BSofSciHitData::R3BSofSciHitData(Int_t sci, Double_t x, Double_t time)
    : fSciId(sci)
    , fX(x)
    , fTime(time)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofSciHitData)
