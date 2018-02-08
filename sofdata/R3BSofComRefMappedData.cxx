#include "R3BSofComRefMappedData.h"

// for the data analysis of the COMMON REFERENCE TIME SIGNAL sent to S2 and Cave C for the reconstruction of the ToF of the secondary beam
// Introduced by Audrey, April 2017


R3BSofComRefMappedData::R3BSofComRefMappedData()
    : fDetector(0)
    , fTimeCoarse(0)
    , fTimeFine(0)
    , fValid(kFALSE)
{
}

R3BSofComRefMappedData::R3BSofComRefMappedData(UShort_t detector,
					       UInt_t tc,
					       UInt_t tf,
					       Bool_t valid)
    : fDetector(detector)
    , fTimeCoarse(tc)
    , fTimeFine(tf)
    , fValid(valid)
{
}

ClassImp(R3BSofComRefMappedData)
