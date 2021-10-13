#include "R3BSofSciSingleTcalData.h"

R3BSofSciSingleTcalData::R3BSofSciSingleTcalData() {}

R3BSofSciSingleTcalData::R3BSofSciSingleTcalData(UShort_t det,
                                                 Double_t time,
                                                 Double_t pos,
                                                 Double_t tofS2,
                                                 Double_t tofS8)
    : fDetector(det)
    , fRawTimeNs(time)
    , fRawPosNs(pos)
    , fS2RawTofNs(tofS2)
    , fS8RawTofNs(tofS8)
{
}

ClassImp(R3BSofSciSingleTcalData)
