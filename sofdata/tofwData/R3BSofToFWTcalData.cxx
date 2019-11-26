#include "R3BSofToFWTcalData.h"

R3BSofToFWTcalData::R3BSofToFWTcalData()
    : fDetector(0)
    , fPmt(0)
    , fRawTimeNs(0)
{
}

R3BSofToFWTcalData::R3BSofToFWTcalData(UShort_t detector, UShort_t pmt, Double_t tns)
    : fDetector(detector)
    , fPmt(pmt)
    , fRawTimeNs(tns)
{
}
ClassImp(R3BSofToFWTcalData)
