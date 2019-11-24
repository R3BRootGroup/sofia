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
    R3BSofTwimHitData(UShort_t secID, Double_t theta, Double_t z);

    /** Destructor **/
    virtual ~R3BSofTwimHitData() {}

    /** Accessors **/
    inline const UShort_t& GetSecID() const { return fSecID; }
    inline const Double_t& GetTheta() const { return fTheta; }
    inline const Double_t& GetZcharge() const { return fZ; }

    /** Modifiers **/
    void SetSecID(UShort_t id) { fSecID = id; };
    void SetTheta(Double_t theta) { fTheta = theta; };
    void SetZcharge(Double_t z) { fZ = z; };

  protected:
    UShort_t fSecID;
    Double_t fTheta, fZ;

    ClassDef(R3BSofTwimHitData, 1)
};

#endif
