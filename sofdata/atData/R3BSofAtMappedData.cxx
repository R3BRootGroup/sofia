// -------------------------------------------------------------------------
// -----                      R3BSofAtMappedData source file           -----
// -------------------------------------------------------------------------

#include "R3BSofAtMappedData.h"

// -----   Default constructor   -------------------------------------------
R3BSofAtMappedData::R3BSofAtMappedData()
    : fSecID(0)
    , fEnergy(0)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
R3BSofAtMappedData::R3BSofAtMappedData(UChar_t secID, UShort_t energy)
    : fSecID(secID)
    , fEnergy(energy)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofAtMappedData)
