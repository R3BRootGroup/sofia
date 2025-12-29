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
// -----                     R3BSofTofWPoint header file               -----
// -----             Created 06/12/17 by J.L. Rodriguez-Sanchez        -----
// -------------------------------------------------------------------------

#pragma once

#include <FairMCPoint.h>
#include <TObject.h>
#include <TVector3.h>

class R3BSofTofWPoint : public FairMCPoint
{
  public:
    /** Default constructor **/
    R3BSofTofWPoint();

    /** Constructor with arguments
     *@param trackID  Index of MCTrack
     *@param detID    Detector ID
     *@param detVolID Detector Copy ID
     *@param Z        Atomic number fragment
     *@param A        Mass number fragment
     *@param posIn    Ccoordinates at entrance to active volume [cm]
     *@param posOut   Coordinates at exit of active volume [cm]
     *@param momIn    Momentum of track at entrance [GeV]
     *@param momOut   Momentum of track at exit [GeV]
     *@param tof      Time since event start [ns]
     *@param length   Track length since creation [cm]
     *@param eLoss    Energy deposit [GeV]
     **/
    R3BSofTofWPoint(int trackID,
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
                    double eLoss);

    /** Copy constructor **/
    R3BSofTofWPoint(const R3BSofTofWPoint& point) { *this = point; };

    /** Destructor **/
    virtual ~R3BSofTofWPoint() = default;

    /** Accessors **/
    int GetDetCopyID() const { return fDetCopyID; }
    double GetXIn() const { return fX; }
    double GetYIn() const { return fY; }
    double GetZIn() const { return fZ; }
    double GetXOut() const { return fX_out; }
    double GetYOut() const { return fY_out; }
    double GetZOut() const { return fZ_out; }
    double GetPxOut() const { return fPx_out; }
    double GetPyOut() const { return fPy_out; }
    double GetPzOut() const { return fPz_out; }
    double GetZFF() const { return fZFF; }
    double GetAFF() const { return fAFF; }

    void PositionIn(TVector3& pos) { pos.SetXYZ(fX, fY, fZ); }
    void PositionOut(TVector3& pos) { pos.SetXYZ(fX_out, fY_out, fZ_out); }
    void MomentumOut(TVector3& mom) { mom.SetXYZ(fPx_out, fPy_out, fPz_out); }

    /** Point coordinates at given z from linear extrapolation **/
    double GetX(double z) const;
    double GetY(double z) const;

    /** Check for distance between in and out **/
    bool IsUsable() const;

    /** Modifiers **/
    void SetPositionOut(TVector3 pos);
    void SetMomentumOut(TVector3 mom);
    void SetDetCopyID(int id) { fDetCopyID = id; };

    /** Output to screen **/
    virtual void Print(const Option_t*) const;

  private:
    double fX_out = 0., fY_out = 0., fZ_out = 0.;
    double fPx_out = 0., fPy_out = 0., fPz_out = 0.;
    int fDetCopyID = 0;
    double fZFF = 0., fAFF = 0.;

  public:
    ClassDef(R3BSofTofWPoint, 1);
};

inline void R3BSofTofWPoint::SetPositionOut(TVector3 pos)
{
    fX_out = pos.X();
    fY_out = pos.Y();
    fZ_out = pos.Z();
}

inline void R3BSofTofWPoint::SetMomentumOut(TVector3 mom)
{
    fPx_out = mom.Px();
    fPy_out = mom.Py();
    fPz_out = mom.Pz();
}
