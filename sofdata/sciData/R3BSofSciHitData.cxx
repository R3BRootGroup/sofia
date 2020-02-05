// -------------------------------------------------------------------------
// -----                     R3BSofSciHitData source file             -----
// -------------------------------------------------------------------------

#include "R3BSofSciHitData.h"

// -----   Default constructor   -------------------------------------------
R3BSofSciHitData::R3BSofSciHitData()
    : fPaddleId(0)
    , fX(0.)
    , fTime(0.)
{
}

// -----   Standard constructor   ------------------------------------------
R3BSofSciHitData::R3BSofSciHitData(Int_t paddle, Double_t x, Double_t time)
    : fPaddleId(paddle)
    , fX(x)
    , fTime(time)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofSciHitData)
