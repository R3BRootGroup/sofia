// -------------------------------------------------------------------------
// -----                      R3BSofCorrvMappedData source file         -----
// -------------------------------------------------------------------------

#include "R3BSofCorrvMappedData.h"

// -----   Default constructor   -------------------------------------------
R3BSofCorrvMappedData::R3BSofCorrvMappedData()
    : fCoarseTimeCorr(0)
    , fFineTimeCorr(0)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
R3BSofCorrvMappedData::R3BSofCorrvMappedData(UShort_t ctc, UShort_t ftc)
    : fCoarseTimeCorr(ctc)
    , fFineTimeCorr(ftc)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofCorrvMappedData)
