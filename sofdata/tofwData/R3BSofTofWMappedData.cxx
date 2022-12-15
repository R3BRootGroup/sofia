#include "R3BSofTofWMappedData.h"

// for the data analysis of the SOFIA TIME-OF-FLIGHT WALL

R3BSofTofWMappedData::R3BSofTofWMappedData()
    : fDetector(0)
    , fPmt(0)
    , fTimeCoarse(0)
    , fTimeFine(0)
    , fEnergy(0)
    , fFlag(kFALSE)
{}

R3BSofTofWMappedData::R3BSofTofWMappedData(UShort_t detector, UShort_t pmt, UInt_t tc, UInt_t tf, UInt_t e, Bool_t flag)
    : fDetector(detector)
    , fPmt(pmt)
    , fTimeCoarse(tc)
    , fTimeFine(tf)
    , fEnergy(e)
    , fFlag(flag)
{}

ClassImp(R3BSofTofWMappedData)
