// -------------------------------------------------------------------------
// -----                      R3BSofTrimMappedData source file         -----
// -------------------------------------------------------------------------

#include "R3BSofTrimMappedData.h"

// -----   Default constructor   -------------------------------------------
R3BSofTrimMappedData::R3BSofTrimMappedData()
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
R3BSofTrimMappedData::R3BSofTrimMappedData(UShort_t secID,
                                           UShort_t anodeID,
                                           UShort_t time,
                                           UShort_t energy,
                                           Bool_t pu,
                                           Bool_t ov)
    : fSecID(secID)
    , fAnodeID(anodeID)
    , fTime(time)
    , fEnergy(energy)
    , fPileup(pu)
    , fOverflow(ov)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofTrimMappedData)
