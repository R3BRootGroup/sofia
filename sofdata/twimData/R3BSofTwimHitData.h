// -------------------------------------------------------------------------
// -----                      R3BSofTwimHitData header file            -----
// -----                  Created 13/10/19 by JL Rodriguez             -----
// -------------------------------------------------------------------------

#ifndef R3BSofTwimHitData_H
#define R3BSofTwimHitData_H
#include "TObject.h"

class R3BSofTwimHitData : public TObject
{

  public:
    /** Default constructor **/
    R3BSofTwimHitData();

    /** Constructor with arguments
     *@param secID    Section ID
     *@param theta    Angle theta in [mrad]
     *@param z        Atomic number Z in charge units
     **/
    R3BSofTwimHitData(Int_t secID, Double_t theta, Double_t z);
    R3BSofTwimHitData(Int_t secID, Double_t theta, Double_t z, Double_t ene);

    /** Destructor **/
    virtual ~R3BSofTwimHitData() {}

    /** Accessors **/
    inline const Int_t& GetSecID() const { return fSecID; }
    inline const Double_t& GetTheta() const { return fTheta; }
    inline const Double_t& GetZcharge() const { return fZ; }
    inline const Double_t& GetEave() const { return fE; }

    /** Modifiers **/
    void SetSecID(Int_t id) { fSecID = id; };
    void SetTheta(Double_t theta) { fTheta = theta; };
    void SetZcharge(Double_t z) { fZ = z; };
    void SetEave(Double_t ene) { fE = ene; };

  protected:
    Int_t fSecID;
    Double_t fTheta, fZ, fE;

  public:
    ClassDef(R3BSofTwimHitData, 1)
};

#endif
