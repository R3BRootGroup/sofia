#ifndef R3BSofMwpc0HitData_H
#define R3BSofMwpc0HitData_H
#include "TObject.h"

class R3BSofMwpc0HitData : public TObject {

public:
  /** Default constructor **/
  R3BSofMwpc0HitData();


  /** Constructor with arguments
   *@param fX  Position X in [mm]   
   *@param fY  Position Y in [mm]
   **/
  R3BSofMwpc0HitData(Double_t X, Double_t Y);


  /** Destructor **/
  virtual ~R3BSofMwpc0HitData() { }


  /** Accessors **/
  inline const Double_t&  GetX()  const { return fX; }
  inline const Double_t&  GetY()  const { return fY; }


  /** Modifiers **/
  void SetX(Double_t x)   { fX = x; };
  void SetY(Double_t y)   { fY = y; };


 protected:

  Double_t fX, fY;

  ClassDef(R3BSofMwpc0HitData,1)
};

#endif
