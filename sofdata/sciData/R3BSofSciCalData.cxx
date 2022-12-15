#include "R3BSofSciCalData.h"

// Default Constructor
R3BSofSciCalData::R3BSofSciCalData()
    : fDetector(1)
    , fPosMm(-1500)
    , fBeta_S2(0)
    , fBeta_S8(0)
    , fTofNs_S2(-1)
    , fTofNs_S8(-1)
{}
R3BSofSciCalData::R3BSofSciCalData(UShort_t det, Double_t x, Double_t b2, Double_t b8, Double_t t2, Double_t t8)
    : fDetector(det)
    , fPosMm(x)
    , fBeta_S2(b2)
    , fBeta_S8(b8)
    , fTofNs_S2(t2)
    , fTofNs_S8(t8)
{}

// Destructor
R3BSofSciCalData::~R3BSofSciCalData() {}

ClassImp(R3BSofSciCalData)
