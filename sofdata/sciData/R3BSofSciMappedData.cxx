#include "R3BSofSciMappedData.h"

R3BSofSciMappedData::R3BSofSciMappedData()
    : fDetector(0)
    , fPmt(0)
    , fTimeCoarse(0)
    , fTimeFine(0)
{}

R3BSofSciMappedData::R3BSofSciMappedData(UShort_t detector, UShort_t pmt, UInt_t tc, UInt_t tf)
    : fDetector(detector)
    , fPmt(pmt)
    , fTimeCoarse(tc)
    , fTimeFine(tf)
{}

ClassImp(R3BSofSciMappedData)
