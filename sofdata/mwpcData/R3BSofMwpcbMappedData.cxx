// -------------------------------------------------------------------------
// -----                     R3BSofMwpcbMappedData source file         -----
// -------------------------------------------------------------------------

#include "R3BSofMwpcbMappedData.h"


// -----   Default constructor   -------------------------------------------
R3BSofMwpcbMappedData::R3BSofMwpcbMappedData() 
: fPlane(0),
  fPad(0),
  fCharge(0) 
{
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
R3BSofMwpcbMappedData::R3BSofMwpcbMappedData(UChar_t plane, UChar_t pad,
					     UShort_t charge) 
  : fPlane(plane)
  , fPad(pad)
  , fCharge(charge)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofMwpcbMappedData)
