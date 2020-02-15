#include "R3BSofToFWSingleTcalData.h"

R3BSofToFWSingleTcalData::R3BSofToFWSingleTcalData()
    : fDetector(0)
    , fRawTimeNs(0.)
    , fRawTofNs(0.)
    , fRawPosNs(0.)
{
}

R3BSofToFWSingleTcalData::R3BSofToFWSingleTcalData(Int_t detector, Double_t time, Double_t tof, Double_t pos)
    : fDetector(detector)
    , fRawTimeNs(time)
    , fRawTofNs(tof)
    , fRawPosNs(pos)
{
}
ClassImp(R3BSofToFWSingleTcalData)
