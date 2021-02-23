// -------------------------------------------------------------------------
// -----                 R3BSofTrimHitData source file         -----
// -------------------------------------------------------------------------

#include "R3BSofTrimHitData.h"

// -----   Default constructor   -------------------------------------------
R3BSofTrimHitData::R3BSofTrimHitData()
    : fSecID(0)
    , fEnergyRaw(0.)
    , fEnergyBeta(0.)
    , fEnergyDT(0.)
    , fEnergyTheta(0.)
    , fZcharge(0.)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructors   ------------------------------------------
R3BSofTrimHitData::R3BSofTrimHitData(Int_t secID)
    : fSecID(secID)
    , fEnergyRaw(0.)
    , fEnergyBeta(0.)
    , fEnergyDT(0.)
    , fEnergyTheta(0.)
    , fZcharge(0.)
{
}

R3BSofTrimHitData::R3BSofTrimHitData(Int_t secID, Float_t Eraw, Float_t Ebeta, Float_t Edt, Float_t Etheta, Float_t Z)
    : fSecID(secID)
    , fEnergyRaw(Eraw)
    , fEnergyBeta(Ebeta)
    , fEnergyDT(Edt)
    , fEnergyTheta(Etheta)
    , fZcharge(Z)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofTrimHitData)
