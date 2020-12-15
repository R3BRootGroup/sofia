// -------------------------------------------------------------------------
// -----                     R3BSofTofWHitData source file             -----
// -------------------------------------------------------------------------

#include "R3BSofTofWHitData.h"

// -----   Default constructor   -------------------------------------------
R3BSofTofWHitData::R3BSofTofWHitData()
    : fPaddleId(0)
    , fX(0.)
    , fY(0.)
    , fTime(0.)
    , fBrho(0.)
    , fvel(0.)
{
}

// -----   Standard constructor   ------------------------------------------
R3BSofTofWHitData::R3BSofTofWHitData(Int_t paddle, Double_t x, Double_t y, Double_t time, Double_t brho, Double_t vel)
    : fPaddleId(paddle)
    , fX(x)
    , fY(y)
    , fTime(time)
    , fBrho(brho) // Reserved for the future development but in principle it's not possible to reconstruct Brho with
                  // TofW only.
    , fvel(vel)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofTofWHitData)
