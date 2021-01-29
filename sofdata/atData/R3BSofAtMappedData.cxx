// -------------------------------------------------------------------------
// -----                      R3BSofAtMappedData source file           -----
// -------------------------------------------------------------------------

#include "R3BSofAtMappedData.h"

// -----   Default constructor   -------------------------------------------
R3BSofAtMappedData::R3BSofAtMappedData()
    : fAnodeID(0)
    , fEnergy(0)
    , fTime(0)
    , fPileup(kFALSE)
    , fOverflow(kFALSE)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
R3BSofAtMappedData::R3BSofAtMappedData(UShort_t a, UShort_t e, UShort_t t, Bool_t pu, Bool_t ov)
    : fAnodeID(a)
    , fEnergy(e)
    , fTime(t)
    , fPileup(pu)
    , fOverflow(ov)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofAtMappedData)
