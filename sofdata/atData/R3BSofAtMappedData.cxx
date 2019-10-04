// -------------------------------------------------------------------------
// -----                      R3BSofAtMappedData source file           -----
// -------------------------------------------------------------------------

#include "R3BSofAtMappedData.h"


// -----   Default constructor   -------------------------------------------
R3BSofAtMappedData::R3BSofAtMappedData() 
  : fEnergy(0) 
{
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
R3BSofAtMappedData::R3BSofAtMappedData(UShort_t energy) 
  : fEnergy(energy)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofAtMappedData)
