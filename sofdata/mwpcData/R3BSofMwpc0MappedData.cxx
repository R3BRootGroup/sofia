// -------------------------------------------------------------------------
// -----                     R3BSofMwpc0MappedData source file         -----
// -------------------------------------------------------------------------

#include "R3BSofMwpc0MappedData.h"


// -----   Default constructor   -------------------------------------------
R3BSofMwpc0MappedData::R3BSofMwpc0MappedData() 
: fPlane(0),
  fPad(0),
  fCharge(0) 
{
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
R3BSofMwpc0MappedData::R3BSofMwpc0MappedData(UChar_t plane, UChar_t pad,
					     UShort_t charge) 
  : fPlane(plane)
  , fPad(pad)
  , fCharge(charge)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofMwpc0MappedData)
