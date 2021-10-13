// ---------------------------------------------------------------------------
// -----                                                                 -----
// -----                      R3BSofTrackingData                         -----
// -----                  Created 09/02/2020 by J.L. Rodriguez           -----
// -----                                                                 -----
// ---------------------------------------------------------------------------

#include "R3BSofTrackingData.h"

R3BSofTrackingData::R3BSofTrackingData()
    : fZ(0.)
    , fAq(0.)
    , fBeta(0.)
    , fLength(0.)
    , fBrho(0.)
    , fPaddle(0)
{
}

//------------------------------

R3BSofTrackingData::R3BSofTrackingData(Double_t z,
                                       Double_t aq,
                                       Double_t beta,
                                       Double_t length,
                                       Double_t brho,
                                       Int_t paddle)
    : fZ(z)
    , fAq(aq)
    , fBeta(beta)
    , fLength(length)
    , fBrho(brho)
    , fPaddle(paddle)
{
}

ClassImp(R3BSofTrackingData)
