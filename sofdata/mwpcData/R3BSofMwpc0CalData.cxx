// -------------------------------------------------------------------------
// -----                     R3BSofMwpc0CalData source file            -----
// -------------------------------------------------------------------------

#include "R3BSofMwpc0CalData.h"


// -----   Default constructor   -------------------------------------------
R3BSofMwpc0CalData::R3BSofMwpc0CalData() 
: fPlane(0),
  fPad(0),
  fCharge(0) 
{
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
R3BSofMwpc0CalData::R3BSofMwpc0CalData(UChar_t plane, UChar_t pad,
					     UShort_t charge) 
  : fPlane(plane)
  , fPad(pad)
  , fCharge(charge)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofMwpc0CalData)
