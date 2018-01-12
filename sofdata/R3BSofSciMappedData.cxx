#include "R3BSofSciMappedData.h"

// for the data analysis of the SOFIA SINGLE SCINTILLATORS
// Introduced by Audrey, April 2017


R3BSofSciMappedData::R3BSofSciMappedData()
    : fDetector(0)
    , fPmt(0)
    , fTimeCoarse(0)
    , fTimeFine(0)
{
}

R3BSofSciMappedData::R3BSofSciMappedData(UChar_t detector,
					 UChar_t pmt,
					 UInt_t tc,
					 UInt_t tf)
    : fDetector(detector)
    , fPmt(pmt)
    , fTimeCoarse(tc)
    , fTimeFine(tf)
{
}

ClassImp(R3BSofSciMappedData)
