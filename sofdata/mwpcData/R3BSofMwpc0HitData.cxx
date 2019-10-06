// -------------------------------------------------------------------------
// -----                     R3BSofMwpc0HitData source file         -----
// -------------------------------------------------------------------------

#include "R3BSofMwpc0HitData.h"


// -----   Default constructor   -------------------------------------------
R3BSofMwpc0HitData::R3BSofMwpc0HitData() 
: fX(0.),
  fY(0.)
{
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
R3BSofMwpc0HitData::R3BSofMwpc0HitData(Double_t x, Double_t y) 
  : fX(x)
  , fY(y)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofMwpc0HitData)
