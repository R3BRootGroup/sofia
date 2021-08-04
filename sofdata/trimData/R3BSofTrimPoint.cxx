// -------------------------------------------------------------------------
// -----                      R3BSofTrimPoint source file              -----
// -------------------------------------------------------------------------

#include "R3BSofTrimPoint.h"

#include <iostream>

// -----   Default constructor   -------------------------------------------
R3BSofTrimPoint::R3BSofTrimPoint()
    : FairMCPoint()
    , fX_out(0.)
    , fY_out(0.)
    , fZ_out(0.)
    , fPx_out(0.)
    , fPy_out(0.)
    , fPz_out(0.)
    , fZF(0)
    , fAF(0)
{
}

// -----   Standard constructor   ------------------------------------------
R3BSofTrimPoint::R3BSofTrimPoint(Int_t trackID,
                                 Int_t detID,
                                 Int_t detCopyID,
                                 Double_t zf,
                                 Double_t af,
                                 TVector3 posIn,
                                 TVector3 posOut,
                                 TVector3 momIn,
                                 TVector3 momOut,
                                 Double_t tof,
                                 Double_t length,
                                 Double_t eLoss)
    : FairMCPoint(trackID, detID, posIn, momIn, tof, length, eLoss)
{
    fDetCopyID = detCopyID;
    fX_out = posOut.X();
    fY_out = posOut.Y();
    fZ_out = posOut.Z();
    fPx_out = momOut.Px();
    fPy_out = momOut.Py();
    fPz_out = momOut.Pz();
    fZF = zf;
    fAF = af;
}

// -----   Destructor   ----------------------------------------------------
R3BSofTrimPoint::~R3BSofTrimPoint() {}

// -----   Point x coordinate from linear extrapolation   ------------------
Double_t R3BSofTrimPoint::GetX(Double_t z) const
{
    if ((fZ_out - z) * (fZ - z) >= 0.)
        return (fX_out + fX) / 2.;
    Double_t dz = fZ_out - fZ;
    return (fX + (z - fZ) / dz * (fX_out - fX));
}

// -----   Point y coordinate from linear extrapolation   ------------------
Double_t R3BSofTrimPoint::GetY(Double_t z) const
{
    if ((fZ_out - z) * (fZ - z) >= 0.)
        return (fY_out + fY) / 2.;
    Double_t dz = fZ_out - fZ;
    return (fY + (z - fZ) / dz * (fY_out - fY));
}

// -----   Public method IsUsable   ----------------------------------------
Bool_t R3BSofTrimPoint::IsUsable() const
{
    Double_t dz = fZ_out - fZ;
    if (TMath::Abs(dz) < 1.e-4)
        return kFALSE;
    return kTRUE;
}

ClassImp(R3BSofTrimPoint);
