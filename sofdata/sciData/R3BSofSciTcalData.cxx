#include "R3BSofSciTcalData.h"

R3BSofSciTcalData::R3BSofSciTcalData()
    : fDetector(0)
    , fPmt(0)
    , fRawTimeNs(0)
{
}

R3BSofSciTcalData::R3BSofSciTcalData(UShort_t detector, UShort_t pmt, Double_t tns)
    : fDetector(detector)
    , fPmt(pmt)
    , fRawTimeNs(tns)
{
}
ClassImp(R3BSofSciTcalData)
