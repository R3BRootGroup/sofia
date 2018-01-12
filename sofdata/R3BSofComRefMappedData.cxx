#include "R3BSofComRefMappedData.h"

// for the data analysis of the COMMON REFERENCE TIME SIGNAL sent to S2 and Cave C for the reconstruction of the ToF of the secondary beam
// Introduced by Audrey, April 2017


R3BSofComRefMappedData::R3BSofComRefMappedData()
    : fDetector(0)
    , fTimeCoarse(0)
    , fTimeFine(0)
{
}

R3BSofComRefMappedData::R3BSofComRefMappedData(UChar_t detector,
					       UInt_t tc,
					       UInt_t tf)
    : fDetector(detector)
    , fTimeCoarse(tc)
    , fTimeFine(tf)
{
}

ClassImp(R3BSofComRefMappedData)
