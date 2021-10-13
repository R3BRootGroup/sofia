// -------------------------------------------------------------------------
// -----                      R3BSofTwimCalData header file            -----
// -----                  Created 29/09/19 by JL Rodriguez             -----
// -------------------------------------------------------------------------

#ifndef R3BSofTwimCalData_H
#define R3BSofTwimCalData_H
#include "TObject.h"

class R3BSofTwimCalData : public TObject
{

  public:
    /** Default constructor **/
    R3BSofTwimCalData();

    /** Constructor with arguments
     *@param secID    Section ID
     *@param anodeID  Anode ID
     *@param dtime    Drift time calibrated in [mm] for the position
     *@param eLoss    Energy deposit [channels], for simulation in [MeV]
     **/
    R3BSofTwimCalData(Int_t secID, Int_t anodeID, Double_t dtime, Double_t energy);

    /** Destructor **/
    virtual ~R3BSofTwimCalData() {}

    /** Accessors **/
    inline const Int_t& GetSecID() const { return fSecID; }
    inline const Int_t& GetAnodeID() const { return fAnodeID; }
    inline const Double_t& GetDTime() const { return fDT; }
    inline const Double_t& GetEnergy() const { return fEnergy; }

    /** Modifiers **/
    void SetSecID(Int_t id) { fSecID = id; };
    void SetAnodeID(Int_t id) { fAnodeID = id; };
    void SetDTime(Double_t dt) { fDT = dt; };
    void SetEnergy(Double_t energy) { fEnergy = energy; };

  protected:
    Int_t fSecID, fAnodeID;
    Double_t fDT, fEnergy;

  public:
    ClassDef(R3BSofTwimCalData, 1)
};

#endif
