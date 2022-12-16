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
    R3BSofTrimHitData(Int_t secID,
                      Float_t Ep1,
                      Float_t Ep2,
                      Float_t Ep3,
                      Float_t Eraw,
                      Float_t Ebeta,
                      Float_t Etheta,
                      Float_t Edt,
                      Float_t Z);

    /** Destructor **/
    virtual ~R3BSofTrimHitData() {}

    /** Accessors **/
    inline const Int_t& GetSecID() const { return fSecID; }

    inline const Float_t& GetEnergyPair1() const { return fEnergyPair1; }
    inline const Float_t& GetEnergyPair2() const { return fEnergyPair2; }
    inline const Float_t& GetEnergyPair3() const { return fEnergyPair3; }
    inline const Float_t& GetEnergyRaw() const { return fEnergyRaw; }
    inline const Float_t& GetEnergyBeta() const { return fEnergyBeta; }
    inline const Float_t& GetEnergyTheta() const { return fEnergyTheta; }
    inline const Float_t& GetEnergyDT() const { return fEnergyDT; }
    inline const Float_t& GetZcharge() const { return fZcharge; }

    /** Modifiers **/
    void SetSecID(Int_t id) { fSecID = id; };
    void SetEnergyPair1(Float_t e) { fEnergyPair1 = e; }
    void SetEnergyPair2(Float_t e) { fEnergyPair2 = e; }
    void SetEnergyPair3(Float_t e) { fEnergyPair3 = e; }
    void SetEnergyRaw(Float_t e) { fEnergyRaw = e; };
    void SetEnergyBeta(Float_t e) { fEnergyBeta = e; };
    void SetEnergyTheta(Float_t e) { fEnergyTheta = e; };
    void SetEnergyDT(Float_t e) { fEnergyDT = e; };
    void SetZcharge(Float_t z) { fZcharge = z; }

  protected:
    Int_t fSecID;
    Float_t fEnergyPair1;
    Float_t fEnergyPair2;
    Float_t fEnergyPair3;
    Float_t fEnergyRaw;
    Float_t fEnergyBeta;
    Float_t fEnergyTheta;
    Float_t fEnergyDT;
    Float_t fZcharge;

    ClassDef(R3BSofTrimHitData, 1)
};

#endif
