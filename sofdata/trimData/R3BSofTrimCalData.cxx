// -------------------------------------------------------------------------
// -----                 R3BSofTrimCalData source file         -----
// -------------------------------------------------------------------------

#include "R3BSofTrimCalData.h"

// -----   Default constructor   -------------------------------------------
R3BSofTrimCalData::R3BSofTrimCalData()
    : fSecID(0)
    , fAnodeID(0)
    , fDriftTimeRaw(-1000000)
    , fDriftTimeAligned(-1000000)
    , fEnergySub(0)
    , fEnergyMatch(0)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructors   ------------------------------------------
R3BSofTrimCalData::R3BSofTrimCalData(Int_t secID, Int_t anodeID)
    : fSecID(secID)
    , fAnodeID(anodeID)
    , fDriftTimeRaw(-1000000)
    , fDriftTimeAligned(-1000000)
    , fEnergySub(0)
    , fEnergyMatch(0)
{
}

R3BSofTrimCalData::R3BSofTrimCalData(Int_t secID,
                                     Int_t anodeID,
                                     Double_t dtraw,
                                     Double_t dtal,
                                     Float_t esub,
                                     Float_t ematch)
    : fSecID(secID)
    , fAnodeID(anodeID)
    , fDriftTimeRaw(dtraw)
    , fDriftTimeAligned(dtal)
    , fEnergySub(esub)
    , fEnergyMatch(ematch)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofTrimCalData)
