/******************************************************************************
 *   Copyright (C) 2017 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2017-2026 Members of R3B Collaboration                     *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

// -------------------------------------------------------------------------
// -----                      R3BSofTofWPoint source file              -----
// -------------------------------------------------------------------------

#include "R3BSofTofWPoint.h"

#include <iostream>

using std::cout;
using std::endl;

// -----   Default constructor   -------------------------------------------
R3BSofTofWPoint::R3BSofTofWPoint()
    : FairMCPoint()
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
R3BSofTofWPoint::R3BSofTofWPoint(int trackID,
                                 int detID,
                                 int detCopyID,
                                 double Z,
                                 double A,
                                 TVector3 posIn,
                                 TVector3 posOut,
                                 TVector3 momIn,
                                 TVector3 momOut,
                                 double tof,
                                 double length,
                                 double eLoss)
    : FairMCPoint(trackID, detID, posIn, momIn, tof, length, eLoss)
{
    fDetCopyID = detCopyID;
    fX_out = posOut.X();
    fY_out = posOut.Y();
    fZ_out = posOut.Z();
    fPx_out = momOut.Px();
    fPy_out = momOut.Py();
    fPz_out = momOut.Pz();
    fZFF = Z;
    fAFF = A;
}

// -----   Public method Print   -------------------------------------------
void R3BSofTofWPoint::Print(const Option_t*) const
{
    cout << "R3BSofTofWPoint: STS Point for track " << fTrackID << " in detector " << fDetectorID << endl;
    cout << "    Position (" << fX << ", " << fY << ", " << fZ << ") cm" << endl;
    cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz << ") GeV" << endl;
    cout << "    Time " << fTime << " ns,  Length " << fLength << " cm,  Energy loss " << fELoss * 1.0e06 << " keV"
         << endl;
}
// -------------------------------------------------------------------------

// -----   Point x coordinate from linear extrapolation   ------------------
double R3BSofTofWPoint::GetX(double z) const
{
    if ((fZ_out - z) * (fZ - z) >= 0.)
        return (fX_out + fX) / 2.;
    double dz = fZ_out - fZ;
    return (fX + (z - fZ) / dz * (fX_out - fX));
}
// -------------------------------------------------------------------------

// -----   Point y coordinate from linear extrapolation   ------------------
double R3BSofTofWPoint::GetY(double z) const
{
    if ((fZ_out - z) * (fZ - z) >= 0.)
        return (fY_out + fY) / 2.;
    double dz = fZ_out - fZ;
    return (fY + (z - fZ) / dz * (fY_out - fY));
}
// -------------------------------------------------------------------------

// -----   Public method IsUsable   ----------------------------------------
bool R3BSofTofWPoint::IsUsable() const
{
    double dz = fZ_out - fZ;
    if (TMath::Abs(dz) < 1.e-4)
        return kFALSE;
    return kTRUE;
}
// -------------------------------------------------------------------------

ClassImp(R3BSofTofWPoint)
