// -------------------------------------------------------------------------
// -----                      R3BVftxTrigMappedData source file         -----
// -------------------------------------------------------------------------

#include "R3BVftxTrigMappedData.h"

// -----   Default constructor   -------------------------------------------
R3BVftxTrigMappedData::R3BVftxTrigMappedData()
    : fModuleID(0)
    , fTimeTrig(0)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
R3BVftxTrigMappedData::R3BVftxTrigMappedData(UShort_t id, UShort_t t)
    : fModuleID(id)
    , fTimeTrig(t)
{
}
// -------------------------------------------------------------------------

ClassImp(R3BVftxTrigMappedData)
