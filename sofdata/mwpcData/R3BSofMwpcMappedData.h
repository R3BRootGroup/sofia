#ifndef R3BSofMwpcMappedData_H
#define R3BSofMwpcMappedData_H
#include "TObject.h"

class R3BSofMwpcMappedData : public TObject
{

  public:
    /** Default constructor **/
    R3BSofMwpcMappedData();

    /** Constructor with arguments
     *@param plane   1 for Xdown, 2 for Xup, and 3 for Y
     *@param pad     Pad number
     *@param charge  Charge of each pad [channels]
     **/
    R3BSofMwpcMappedData(UShort_t plane, UShort_t pad, UShort_t charge);

    /** Destructor **/
    virtual ~R3BSofMwpcMappedData() {}

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
    ClassDef(R3BSofMwpcMappedData, 1)
};

#endif
