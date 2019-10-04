// -------------------------------------------------------------------------
// -----                     R3BSofMwpc1MappedData source file         -----
// -------------------------------------------------------------------------

#include "R3BSofMwpc1MappedData.h"


// -----   Default constructor   -------------------------------------------
R3BSofMwpc1MappedData::R3BSofMwpc1MappedData() 
: fPlane(0),
  fPad(0),
  fCharge(0) 
{
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
R3BSofMwpc1MappedData::R3BSofMwpc1MappedData(UChar_t plane, UChar_t pad,
					     UShort_t charge) 
  : fPlane(plane)
  , fPad(pad)
  , fCharge(charge)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofMwpc1MappedData)
