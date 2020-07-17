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
     *@param fTime      Time [ns]
     **/
    R3BSofTofWHitData(Int_t paddle, Double_t X, Double_t Y, Double_t time, Double_t brho=0., Double_t fvel=0.);

    /** Destructor **/
    virtual ~R3BSofTofWHitData() {}

    /** Accessors **/
    inline const Int_t& GetPaddle() const { return fPaddleId; }
    inline const Double_t& GetX() const { return fX; }
    inline const Double_t& GetY() const { return fY; }
    inline const Double_t& GetTime() const { return fTime; }
    inline const Double_t& GetBrho() const { return fBrho; }
    inline const Double_t& GetVel() const { return fvel; }

    /** Modifiers **/
    void SetPaddle(Int_t p) { fPaddleId = p; };
    void SetX(Double_t x) { fX = x; };
    void SetY(Double_t y) { fY = y; };
    void SetTime(Double_t t) { fTime = t; };

  protected:
    Int_t fPaddleId;
    Double_t fX, fY, fTime, fBrho, fvel;

  public:
    ClassDef(R3BSofTofWHitData, 1)
};

#endif
