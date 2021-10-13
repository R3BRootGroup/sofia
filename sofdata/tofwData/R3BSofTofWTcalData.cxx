#include "R3BSofTofWTcalData.h"

R3BSofTofWTcalData::R3BSofTofWTcalData()
    : fDetector(0)
    , fPmt(0)
    , fRawTimeNs(0)
{
}

R3BSofTofWTcalData::R3BSofTofWTcalData(UShort_t detector, UShort_t pmt, Double_t tns)
    : fDetector(detector)
    , fPmt(pmt)
    , fRawTimeNs(tns)
{
}
ClassImp(R3BSofTofWTcalData)
