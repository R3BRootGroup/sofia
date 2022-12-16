#ifndef R3BSofTrimCalData_H
#define R3BSofTrimCalData_H
#include "TObject.h"

class R3BSofTrimCalData : public TObject
{

  public:
    /** Default constructor **/
    R3BSofTrimCalData();

    /** Constructor with arguments
    secID     is 1-based
    anodeID   is 1-based if triangle anodes : 3 pairs of anodes
    **/
    R3BSofTrimCalData(Int_t secID, Int_t anodeID);
    R3BSofTrimCalData(Int_t secID, Int_t anodeID, Double_t dtraw, Double_t dtal, Float_t esub, Float_t ematch);

    /** Destructor **/
    virtual ~R3BSofTrimCalData() {}

    /** Accessors **/
    inline const Int_t& GetSecID() const { return fSecID; }
    inline const Int_t& GetAnodeID() const { return fAnodeID; }

    inline const Double_t& GetDriftTimeRaw() const { return fDriftTimeRaw; }
    inline const Double_t& GetDriftTimeAligned() const { return fDriftTimeAligned; }

    inline const Float_t& GetEnergySub() const { return fEnergySub; }
    inline const Float_t& GetEnergyMatch() const { return fEnergyMatch; }

    /** Modifiers **/
    void SetSecID(Int_t id) { fSecID = id; };
    void SetAnodeID(Int_t id) { fAnodeID = id; };

    void SetDriftTimeRaw(Double_t dt) { fDriftTimeRaw = dt; };
    void SetDriftTimeAligned(Double_t dt) { fDriftTimeAligned = dt; };

    void SetEnergySub(Float_t e) { fEnergySub = e; };
    void SetEnergyMatch(Float_t e) { fEnergyMatch = e; };

  protected:
    Int_t fSecID;
    Int_t fAnodeID;

    Double_t fDriftTimeRaw;
    Double_t fDriftTimeAligned;
    Float_t fEnergySub;
    Float_t fEnergyMatch;

    ClassDef(R3BSofTrimCalData, 1)
};

#endif
