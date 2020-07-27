// -------------------------------------------------------------------------
// -----                      R3BSofTwimHitData source file            -----
// -------------------------------------------------------------------------

#include "R3BSofTwimHitData.h"

// -----   Default constructor   -------------------------------------------
R3BSofTwimHitData::R3BSofTwimHitData()
    : fSecID(0)
    , fTheta(0.)
    , fZ(0.)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
R3BSofTwimHitData::R3BSofTwimHitData(Int_t secID, Double_t theta, Double_t z)
    : fSecID(secID)
    , fTheta(theta)
    , fZ(z)
{
}
// -------------------------------------------------------------------------

// -----   For later analysis with reconstructed beta   --------------------
R3BSofTwimHitData::R3BSofTwimHitData(Int_t secID, Double_t theta, Double_t z, Double_t ene)
  : fSecID(secID)
  , fTheta(theta)
  , fZ(z)
  , fE(ene)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofTwimHitData)
