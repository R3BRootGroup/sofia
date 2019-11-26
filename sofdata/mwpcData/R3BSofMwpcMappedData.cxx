// -------------------------------------------------------------------------
// -----                     R3BSofMwpcMappedData source file          -----
// -------------------------------------------------------------------------

#include "R3BSofMwpcMappedData.h"

// -----   Default constructor   -------------------------------------------
R3BSofMwpcMappedData::R3BSofMwpcMappedData()
    : fPlane(0)
    , fPad(0)
    , fCharge(0)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
R3BSofMwpcMappedData::R3BSofMwpcMappedData(UShort_t plane, UShort_t pad, UShort_t charge)
    : fPlane(plane)
    , fPad(pad)
    , fCharge(charge)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofMwpcMappedData)
