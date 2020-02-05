#ifndef R3BSofSciHitData_H
#define R3BSofSciHitData_H
#include "TObject.h"

class R3BSofSciHitData : public TObject
{

  public:
    /** Default constructor **/
    R3BSofSciHitData();

    /** Constructor with arguments
     *@param fPaddleId  Paddle number
     *@param fX         Position X in [mm]
     *@param fTime      Time [ns]
     **/
    R3BSofSciHitData(Int_t paddle, Double_t X, Double_t time);

    /** Destructor **/
    virtual ~R3BSofSciHitData() {}

    /** Accessors **/
    inline const Int_t& GetPaddle() const { return fPaddleId; }
    inline const Double_t& GetX() const { return fX; }
    inline const Double_t& GetTof() const { return fTime; }

    /** Modifiers **/
    void SetPaddle(Int_t p) { fPaddleId = p; };
    void SetX(Double_t x) { fX = x; };
    void SetTime(Double_t t) { fTime = t; };

  protected:
    Int_t fPaddleId;
    Double_t fX, fTime;

  public:
    ClassDef(R3BSofSciHitData, 1)
};

#endif
