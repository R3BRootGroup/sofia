#ifndef R3BSofTofWHitData_H
#define R3BSofTofWHitData_H
#include "TObject.h"

class R3BSofTofWHitData : public TObject
{

  public:
    /** Default constructor **/
    R3BSofTofWHitData();

    /** Constructor with arguments
     *@param fPaddleId  Paddle number
     *@param fX         Position X in [mm]
     *@param fY         Position Y in [mm]
     *@param fTof      Tof [ns]
     **/
    R3BSofTofWHitData(Int_t paddle, Double_t X, Double_t Y, Double_t tof);

    /** Destructor **/
    virtual ~R3BSofTofWHitData() {}

    /** Accessors **/
    inline const Int_t& GetPaddle() const { return fPaddleId; }
    inline const Double_t& GetX() const { return fX; }
    inline const Double_t& GetY() const { return fY; }
    inline const Double_t& GetTof() const { return fTof; }

    /** Modifiers **/
    void SetPaddle(Int_t p) { fPaddleId = p; };
    void SetX(Double_t x) { fX = x; };
    void SetY(Double_t y) { fY = y; };
    void SetTof(Double_t t) { fTof = t; };

  protected:
    Int_t fPaddleId;
    Double_t fX, fY, fTof;

  public:
    ClassDef(R3BSofTofWHitData, 1)
};

#endif
