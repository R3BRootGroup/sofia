// ---------------------------------------------------------------------------
// -----                                                                 -----
// -----                      R3BSofFrsData                              -----
// -----                  Created 28/01/2020 by J.L. Rodriguez           -----
// -----                                                                 -----
// ---------------------------------------------------------------------------

#include "R3BSofFrsData.h"

R3BSofFrsData::R3BSofFrsData()
    : fZ(0.)
    , fAq(0.)
    , fBeta(0.)
    , fBrho(0.)
{
}

//------------------------------

R3BSofFrsData::R3BSofFrsData(Double_t z, Double_t aq, Double_t beta, Double_t brho)
    : fZ(z)
    , fAq(aq)
    , fBeta(beta)
    , fBrho(brho)
{
}

ClassImp(R3BSofFrsData)
