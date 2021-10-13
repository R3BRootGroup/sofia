#include "R3BSofScalersMappedData.h"

R3BSofScalersMappedData::R3BSofScalersMappedData()
    : fScaler(0)
    , fChannel(0)
    , fValue(0)
{
}

R3BSofScalersMappedData::R3BSofScalersMappedData(UShort_t s, UShort_t c, UInt_t v)
    : fScaler(s)
    , fChannel(c)
    , fValue(v)
{
}

ClassImp(R3BSofScalersMappedData)
