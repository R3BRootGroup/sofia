#ifndef R3BSofMwpcCalData_H
#define R3BSofMwpcCalData_H
#include "TObject.h"

class R3BSofMwpcCalData : public TObject
{

    // This data structure must be filled after pedestal subtraction

  public:
    /** Default constructor **/
    R3BSofMwpcCalData();

    /** Constructor with arguments
     *@param plane   1 for Xup, 2 for Xdown, and 3 for Y
     *@param pad     Pad number
     *@param charge  Charge of each pad [channels]
     **/
    R3BSofMwpcCalData(Int_t plane, Int_t pad, Float_t charge);

    /** Destructor **/
    virtual ~R3BSofMwpcCalData() {}

    /** Accessors **/
    inline const Int_t& GetPlane() const { return fPlane; }
    inline const Int_t& GetPad() const { return fPad; }
    inline const Float_t& GetQ() const { return fCharge; }

    /** Modifiers **/
    void SetPlane(Int_t plane) { fPlane = plane; };
    void SetPad(Int_t pad) { fPad = pad; };
    void SetQ(Float_t charge) { fCharge = charge; };

  protected:
    Int_t fPlane;
    Int_t fPad;
    Float_t fCharge;

  public:
    ClassDef(R3BSofMwpcCalData, 1)
};

#endif
