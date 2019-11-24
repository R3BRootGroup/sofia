// -------------------------------------------------------------------------
// -----                     R3BSofMwpcCalData source file             -----
// -------------------------------------------------------------------------

#include "R3BSofMwpcCalData.h"

// -----   Default constructor   -------------------------------------------
R3BSofMwpcCalData::R3BSofMwpcCalData()
    : fPlane(0)
    , fPad(0)
    , fCharge(0)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
R3BSofMwpcCalData::R3BSofMwpcCalData(UShort_t plane, UShort_t pad, UShort_t charge)
    : fPlane(plane)
    , fPad(pad)
    , fCharge(charge)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofMwpcCalData)
