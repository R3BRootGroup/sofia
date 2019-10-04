// -------------------------------------------------------------------------
// -----                     R3BSofMwpc3MappedData source file         -----
// -------------------------------------------------------------------------

#include "R3BSofMwpc3MappedData.h"


// -----   Default constructor   -------------------------------------------
R3BSofMwpc3MappedData::R3BSofMwpc3MappedData() 
: fPlane(0),
  fPad(0),
  fCharge(0) 
{
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
R3BSofMwpc3MappedData::R3BSofMwpc3MappedData(UChar_t plane, UChar_t pad,
					     UShort_t charge) 
  : fPlane(plane)
  , fPad(pad)
  , fCharge(charge)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofMwpc3MappedData)
