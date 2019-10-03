// -------------------------------------------------------------------------
// -----                      R3BSofTwimCalData source file              -----
// -------------------------------------------------------------------------

#include "R3BSofTwimCalData.h"


// -----   Default constructor   -------------------------------------------
R3BSofTwimCalData::R3BSofTwimCalData()
: fSecID(-1),
  fAnodeID(-1),
  fDT(0.),
  fEnergy(0.) 
{
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
R3BSofTwimCalData::R3BSofTwimCalData(Int_t secID, Int_t anodeID,
			                   Double_t dt, Double_t energy) 
  : fSecID(secID)
  , fAnodeID(anodeID)
  , fDT(dt)
  , fEnergy(energy)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofTwimCalData)
