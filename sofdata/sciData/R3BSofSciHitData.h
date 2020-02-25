#ifndef R3BSofSciHitData_H
#define R3BSofSciHitData_H
#include "TObject.h"

class R3BSofSciHitData : public TObject
{

  public:
    /** Default constructor **/
    R3BSofSciHitData();

    /** Constructor with arguments
     *@param fSciId   Scintillator number
     *@param fX       Position X in [mm]
     *@param fTime    Time [ns]
     **/
    R3BSofSciHitData(Int_t sci, Double_t X, Double_t time);

    /** Destructor **/
    virtual ~R3BSofSciHitData() {}

    /** Accessors **/
    inline const Int_t& GetSciId() const { return fSciId; }
    inline const Double_t& GetX() const { return fX; }
    inline const Double_t& GetTof() const { return fTime; }

    /** Modifiers **/
    void SetSciId(Int_t p) { fSciId = p; };
    void SetX(Double_t x) { fX = x; };
    void SetTime(Double_t t) { fTime = t; };

  protected:
    Int_t fSciId;
    Double_t fX, fTime;

  public:
    ClassDef(R3BSofSciHitData, 1)
};

#endif
