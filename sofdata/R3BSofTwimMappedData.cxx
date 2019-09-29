// -------------------------------------------------------------------------
// -----                      R3BSofTwimMappedData source file              -----
// -------------------------------------------------------------------------

#include "R3BSofTwimMappedData.h"


// -----   Default constructor   -------------------------------------------
R3BSofTwimMappedData::R3BSofTwimMappedData() 
: fSecID(-1),
  fAnodeID(-1),
  fDT(0),
  fEnergy(0) 
{
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
R3BSofTwimMappedData::R3BSofTwimMappedData(Int_t secID, Int_t anodeID,
			                   Int_t dt, Int_t energy) 
  : fSecID(secID)
  , fAnodeID(anodeID)
  , fDT(dt)
  , fEnergy(energy)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofTwimMappedData)
