#include "R3BSofToFWSingleTcalData.h"

R3BSofToFWSingleTcalData::R3BSofToFWSingleTcalData()
    : fDetector(0)
    , fRawTofNs(0)
    , fRawPosNs(0)
{
}

R3BSofToFWSingleTcalData::R3BSofToFWSingleTcalData(UShort_t detector, Double_t tof, Double_t pos)
    : fDetector(detector)
    , fRawTofNs(tof)
    , fRawPosNs(pos)
{
}
ClassImp(R3BSofToFWSingleTcalData)
