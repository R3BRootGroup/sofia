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
{
}

// -----   Standard constructor   ------------------------------------------
R3BSofTofWHitData::R3BSofTofWHitData(UChar_t paddle, Double_t x, Double_t y, Double_t time)
    : fPaddleId(paddle)
    , fX(x)
    , fY(y)
    , fTime(time)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofTofWHitData)
