// -------------------------------------------------------------------------
// -----                      R3BSofTrimPoint header file              -----
// -----                  Created 07/08/21  by JL Rodriguez            -----
// -------------------------------------------------------------------------

/**  R3BSofTrimPoint.h
 **/

#ifndef R3BSofTrimPoint_H
#define R3BSofTrimPoint_H

#include "TObject.h"
#include "TVector3.h"

#include "FairMCPoint.h"

class R3BSofTrimPoint : public FairMCPoint
{

  public:
    /** Default constructor **/
    R3BSofTrimPoint();

    /** Constructor with arguments
     *@param trackID   Index of MCTrack
     *@param detID     Detector ID
     *@param detCopyID Detector Copy ID
     *@param zf        Atomic number fragment
     *@param af        Mass number fragment
     *@param posIn     Coordinates at entrance to active volume [cm]
     *@param posOut    Coordinates at exit of active volume [cm]
     *@param momIn     Momentum of track at entrance [GeV]
     *@param momOut    Momentum of track at exit [GeV]
     *@param time      Time since event start [ns]
     *@param length    Track length since creation [cm]
     *@param eLoss     Energy deposit [GeV]
     **/
    R3BSofTrimPoint(Int_t trackID,
                    Int_t detID,
                    Int_t detCopyID,
                    Double_t zf,
                    Double_t af,
                    TVector3 posIn,
                    TVector3 posOut,
                    TVector3 momIn,
                    TVector3 momOut,
                    Double_t time,
                    Double_t length,
                    Double_t eLoss);

    /** Copy constructor **/
    R3BSofTrimPoint(const R3BSofTrimPoint& point) { *this = point; };

    /** Destructor **/
    virtual ~R3BSofTrimPoint();

    /** Accessors **/
    Int_t GetDetCopyID() const { return fDetCopyID; }
    Double_t GetXIn() const { return fX; }
    Double_t GetYIn() const { return fY; }
    Double_t GetZIn() const { return fZ; }
    Double_t GetXOut() const { return fX_out; }
    Double_t GetYOut() const { return fY_out; }
    Double_t GetZOut() const { return fZ_out; }
    Double_t GetPxOut() const { return fPx_out; }
    Double_t GetPyOut() const { return fPy_out; }
    Double_t GetPzOut() const { return fPz_out; }
    Double_t GetZF() const { return fZF; }
    Double_t GetAF() const { return fAF; }

    void PositionIn(TVector3& pos) { pos.SetXYZ(fX, fY, fZ); }
    void PositionOut(TVector3& pos) { pos.SetXYZ(fX_out, fY_out, fZ_out); }
    void MomentumOut(TVector3& mom) { mom.SetXYZ(fPx_out, fPy_out, fPz_out); }

    /** Point coordinates at given z from linear extrapolation **/
    Double_t GetX(Double_t z) const;
    Double_t GetY(Double_t z) const;

    /** Check for distance between in and out **/
    Bool_t IsUsable() const;

    /** Modifiers **/
    void SetPositionOut(TVector3 pos);
    void SetMomentumOut(TVector3 mom);
    void SetDetCopyID(Int_t id) { fDetCopyID = id; };

  private:
    Double32_t fX_out, fY_out, fZ_out;
    Double32_t fPx_out, fPy_out, fPz_out;
    Int_t fDetCopyID;
    Double_t fZF, fAF;

    ClassDef(R3BSofTrimPoint, 1)
};

inline void R3BSofTrimPoint::SetPositionOut(TVector3 pos)
{
    fX_out = pos.X();
    fY_out = pos.Y();
    fZ_out = pos.Z();
}

inline void R3BSofTrimPoint::SetMomentumOut(TVector3 mom)
{
    fPx_out = mom.Px();
    fPy_out = mom.Py();
    fPz_out = mom.Pz();
}

#endif
