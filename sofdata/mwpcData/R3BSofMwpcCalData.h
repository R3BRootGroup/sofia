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
     *@param plane   1 for Xdown, 2 for Xup, and 3 for Y
     *@param pad     Pad number
     *@param charge  Charge of each pad [channels]
     **/
    R3BSofMwpcCalData(UShort_t plane, UShort_t pad, UShort_t charge);

    /** Destructor **/
    virtual ~R3BSofMwpcCalData() {}

    /** Accessors **/
    inline const UShort_t& GetPlane() const { return fPlane; }
    inline const UShort_t& GetPad() const { return fPad; }
    inline const UShort_t& GetQ() const { return fCharge; }

    /** Modifiers **/
    void SetPlane(UShort_t plane) { fPlane = plane; };
    void SetPad(UShort_t pad) { fPad = pad; };
    void SetQ(UShort_t charge) { fCharge = charge; };

  protected:
    UShort_t fPlane;
    UShort_t fPad;
    UShort_t fCharge;

  public:
    ClassDef(R3BSofMwpcCalData, 1)
};

#endif
