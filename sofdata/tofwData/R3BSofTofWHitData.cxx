// -------------------------------------------------------------------------
// -----                     R3BSofTofWHitData source file             -----
// -------------------------------------------------------------------------

#include "R3BSofTofWHitData.h"

// -----   Default constructor   -------------------------------------------
R3BSofTofWHitData::R3BSofTofWHitData()
    : fPaddleId(0)
    , fX(0.)
    , fY(0.)
    , fTof(0.)
{}

// -----   Standard constructor   ------------------------------------------
R3BSofTofWHitData::R3BSofTofWHitData(Int_t paddle, Double_t x, Double_t y, Double_t tof)
    : fPaddleId(paddle)
    , fX(x)
    , fY(y)
    , fTof(tof)
{}
// -------------------------------------------------------------------------

ClassImp(R3BSofTofWHitData)
