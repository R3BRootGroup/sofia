#ifndef R3BSofMwpcHitData_H
#define R3BSofMwpcHitData_H
#include "TObject.h"

class R3BSofMwpcHitData : public TObject
{

  public:
    /** Default constructor **/
    R3BSofMwpcHitData();

    /** Constructor with arguments
     *@param fX  Position X in [mm]
     *@param fY  Position Y in [mm]
     **/
    R3BSofMwpcHitData(Double_t X, Double_t Y);

    /** Destructor **/
    virtual ~R3BSofMwpcHitData() {}

    /** Accessors **/
    inline const Double_t& GetX() const { return fX; }
    inline const Double_t& GetY() const { return fY; }

    /** Modifiers **/
    void SetX(Double_t x) { fX = x; };
    void SetY(Double_t y) { fY = y; };

  protected:
    Double_t fX, fY;

  public:
    ClassDef(R3BSofMwpcHitData, 1)
};

#endif
