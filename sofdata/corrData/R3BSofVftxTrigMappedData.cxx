// -------------------------------------------------------------------------
// -----                      R3BSofVftxTrigMappedData source file         -----
// -------------------------------------------------------------------------

#include "R3BSofVftxTrigMappedData.h"

// -----   Default constructor   -------------------------------------------
R3BSofVftxTrigMappedData::R3BSofVftxTrigMappedData()
    : fModuleID(0)
    , fTimeTrig(0)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
R3BSofVftxTrigMappedData::R3BSofVftxTrigMappedData(UShort_t id, UShort_t t)
    : fModuleID(id)
    , fTimeTrig(t)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BSofVftxTrigMappedData)
