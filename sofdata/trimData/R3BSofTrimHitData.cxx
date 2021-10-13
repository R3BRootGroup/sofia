// -------------------------------------------------------------------------
// -----                 R3BSofTrimHitData source file         -----
// -------------------------------------------------------------------------

#include "R3BSofTrimHitData.h"

// -----   Default constructor   -------------------------------------------
R3BSofTrimHitData::R3BSofTrimHitData()
    : fSecID(0)
    , fEnergyPair1(0.)
    , fEnergyPair2(0.)
    , fEnergyPair3(0.)
    , fEnergyRaw(0.)
    , fEnergyBeta(0.)
    , fEnergyTheta(0.)
    , fEnergyDT(0.)
    , fZcharge(0.)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructors   ------------------------------------------
R3BSofTrimHitData::R3BSofTrimHitData(Int_t secID)
    : fSecID(secID)
    , fEnergyPair1(0.)
    , fEnergyPair2(0.)
    , fEnergyPair3(0.)
    , fEnergyRaw(0.)
    , fEnergyBeta(0.)
    , fEnergyTheta(0.)
    , fEnergyDT(0.)
    , fZcharge(0.)
{
}

R3BSofTrimHitData::R3BSofTrimHitData(Int_t secID,
                                     Float_t e1,
                                     Float_t e2,
                                     Float_t e3,
                                     Float_t Eraw,
                                     Float_t Ebeta,
                                     Float_t Etheta,
                                     Float_t Edt,
                                     Float_t Z)
    : fSecID(secID)
    , fEnergyPair1(e1)
    , fEnergyPair2(e2)
    , fEnergyPair3(e3)
    , fEnergyRaw(Eraw)
    , fEnergyBeta(Ebeta)
    , fEnergyTheta(Etheta)
    , fEnergyDT(Edt)
    , fZcharge(Z)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofTrimHitData)
