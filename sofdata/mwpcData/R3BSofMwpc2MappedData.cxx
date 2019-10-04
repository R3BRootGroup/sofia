// -------------------------------------------------------------------------
// -----                     R3BSofMwpc2MappedData source file         -----
// -------------------------------------------------------------------------

#include "R3BSofMwpc2MappedData.h"


// -----   Default constructor   -------------------------------------------
R3BSofMwpc2MappedData::R3BSofMwpc2MappedData() 
: fPlane(0),
  fPad(0),
  fCharge(0) 
{
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
R3BSofMwpc2MappedData::R3BSofMwpc2MappedData(UChar_t plane, UChar_t pad,
					     UShort_t charge) 
  : fPlane(plane)
  , fPad(pad)
  , fCharge(charge)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofMwpc2MappedData)
