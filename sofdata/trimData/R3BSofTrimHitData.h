#ifndef R3BSofTrimHitData_H
#define R3BSofTrimHitData_H
#include "TObject.h"

class R3BSofTrimHitData : public TObject
{

  public:
    /** Default constructor **/
    R3BSofTrimHitData();

    /** Constructor with arguments
    secID     is 1-based
    **/
    R3BSofTrimHitData(Int_t secID);
    R3BSofTrimHitData(Int_t secID, Float_t Eraw, Float_t Ebeta, Float_t Edt, Float_t Etheta, Float_t Z);

    /** Destructor **/
    virtual ~R3BSofTrimHitData() {}

    /** Accessors **/
    inline const Int_t& GetSecID() const { return fSecID; }

    inline const Float_t& GetEnergyRaw() const { return fEnergyRaw; }
    inline const Float_t& GetEnergyBeta() const { return fEnergyBeta; }
    inline const Float_t& GetEnergyDT() const { return fEnergyDT; }
    inline const Float_t& GetEnergyTheta() const { return fEnergyTheta; }
    inline const Float_t& GetZ() const { return fZ; }

    /** Modifiers **/
    void SetSecID(Int_t id) { fSecID = id; };
    void SetEnergyRaw(Float_t e) { fEnergyRaw = e; };
    void SetEnergyBeta(Float_t e) { fEnergyBeta = e; };
    void SetEnergyDT(Float_t e) { fEnergyDT = e; };
    void SetEnergyTheta(Float_t e) { fEnergyTheta = e; };
    void SetZ(Float_t z) { fZ = z; }

  protected:
    Int_t fSecID;
    Float_t fEnergyRaw;
    Float_t fEnergyBeta;
    Float_t fEnergyDT;
    Float_t fEnergyTheta;
    Float_t fZ;

    ClassDef(R3BSofTrimHitData, 1)
};

#endif
