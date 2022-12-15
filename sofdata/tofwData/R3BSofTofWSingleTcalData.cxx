#include "R3BSofTofWSingleTcalData.h"

R3BSofTofWSingleTcalData::R3BSofTofWSingleTcalData()
    : fDetector(0)
    , fRawTimeNs(0.)
    , fRawTofNs(0.)
    , fRawPosNs(0.)
{}

R3BSofTofWSingleTcalData::R3BSofTofWSingleTcalData(Int_t detector, Double_t time, Double_t tof, Double_t pos)
    : fDetector(detector)
    , fRawTimeNs(time)
    , fRawTofNs(tof)
    , fRawPosNs(pos)
{}
ClassImp(R3BSofTofWSingleTcalData)
