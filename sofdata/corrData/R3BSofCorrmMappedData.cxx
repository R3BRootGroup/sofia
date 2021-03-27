// -------------------------------------------------------------------------
// -----                      R3BSofCorrmMappedData source file         -----
// -------------------------------------------------------------------------

#include "R3BSofCorrmMappedData.h"

// -----   Default constructor   -------------------------------------------
R3BSofCorrmMappedData::R3BSofCorrmMappedData()
    : fTimeCorr(0)
    , fTimeTrig(0)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
R3BSofCorrmMappedData::R3BSofCorrmMappedData(UShort_t tc, UShort_t tt)
    : fTimeCorr(tc)
    , fTimeTrig(tt)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofCorrmMappedData)
