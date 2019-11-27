#include "R3BSofSciSingleTcalData.h"

R3BSofSciSingleTcalData::R3BSofSciSingleTcalData()
{
}

R3BSofSciSingleTcalData::R3BSofSciSingleTcalData(Double_t t, Double_t p, Double_t m) 
{
  fRawTimeNs[0]  = t;
  fRawPosNs[0]   = p;
  fMultPerDet[0] = m;

}

ClassImp(R3BSofSciSingleTcalData)
