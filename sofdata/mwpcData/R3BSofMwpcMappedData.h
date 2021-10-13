#ifndef R3BSofMwpcMappedData_H
#define R3BSofMwpcMappedData_H
#include "TObject.h"

class R3BSofMwpcMappedData : public TObject
{

  public:
    /** Default constructor **/
    R3BSofMwpcMappedData();

    /** Constructor with arguments
     *@param plane   1 for Xup, 2 for Xdown, and 3 for Y
     *@param pad     Pad number
     *@param charge  Charge of each pad [channels]
     **/
    R3BSofMwpcMappedData(Int_t plane, Int_t pad, Int_t charge);

    /** Destructor **/
    virtual ~R3BSofMwpcMappedData() {}

    /** Accessors **/
    inline const Int_t& GetPlane() const { return fPlane; }
    inline const Int_t& GetPad() const { return fPad; }
    inline const Int_t& GetQ() const { return fCharge; }

    /** Modifiers **/
    void SetPlane(Int_t plane) { fPlane = plane; };
    void SetPad(Int_t pad) { fPad = pad; };
    void SetQ(Int_t charge) { fCharge = charge; };

  protected:
    Int_t fPlane;
    Int_t fPad;
    Int_t fCharge;

  public:
    ClassDef(R3BSofMwpcMappedData, 1)
};

#endif
