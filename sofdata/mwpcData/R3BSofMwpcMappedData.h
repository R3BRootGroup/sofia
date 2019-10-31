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
    R3BSofMwpcMappedData(UChar_t plane, UChar_t pad, UShort_t charge);

    /** Destructor **/
    virtual ~R3BSofMwpcMappedData() {}

    /** Accessors **/
    inline const UChar_t& GetPlane() const { return fPlane; }
    inline const UChar_t& GetPad() const { return fPad; }
    inline const UShort_t& GetQ() const { return fCharge; }

    /** Modifiers **/
    void SetPlane(UChar_t plane) { fPlane = plane; };
    void SetPad(UChar_t pad) { fPad = pad; };
    void SetQ(UShort_t charge) { fCharge = charge; };

  protected:
    UChar_t fPlane;
    UChar_t fPad;
    UShort_t fCharge;

    ClassDef(R3BSofMwpcMappedData, 1)
};

#endif
