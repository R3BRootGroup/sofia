// -------------------------------------------------------------------------
// -----                     R3BSofMwpcHitData source file             -----
// -------------------------------------------------------------------------

#include "R3BSofMwpcHitData.h"

// -----   Default constructor   -------------------------------------------
R3BSofMwpcHitData::R3BSofMwpcHitData()
    : fX(0.)
    , fY(0.)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
R3BSofMwpcHitData::R3BSofMwpcHitData(Double_t x, Double_t y)
    : fX(x)
    , fY(y)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofMwpcHitData)
