// -------------------------------------------------------------------------
// -----            R3BSofTofWCalData source file                      -----
// -------------------------------------------------------------------------

#include "R3BSofTofWCalData.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;

// -----   Default constructor   -------------------------------------------
R3BSofTofWCalData::R3BSofTofWCalData()
    : FairMultiLinkedData()
    , fDetector(0)
    , fPmt(0)
    , fTime(-1.)
    , fEnergy(-1.)
{
}

// -----   Standard constructor   ------------------------------------------
R3BSofTofWCalData::R3BSofTofWCalData(UChar_t ident, UShort_t pmt, Double_t time, Float_t energy)
    : FairMultiLinkedData()
    , fDetector(ident)
    , fPmt(pmt)
    , fTime(time)
    , fEnergy(energy)
{
}

// -----   Copy constructor   ------------------------------------------
R3BSofTofWCalData::R3BSofTofWCalData(const R3BSofTofWCalData& right)
    : FairMultiLinkedData(right)
    , fDetector(right.fDetector)
    , fPmt(right.fPmt)
    , fTime(right.fTime)
    , fEnergy(right.fEnergy)
{
}

// -----   Destructor   ----------------------------------------------------
R3BSofTofWCalData::~R3BSofTofWCalData() {}

// -----   Public method Print   -------------------------------------------
void R3BSofTofWCalData::Print(const Option_t* opt) const
{
    cout << "-I- R3BSofTofWCalData: Sofia TofW Cal Data level for:" << endl;
    cout << "   fDetector " << fDetector << "  fPmt = " << fPmt << endl;
    cout << "    fTime = " << fTime << " (ns)" << endl;
    cout << "    fEnergy = " << fEnergy << " (GeV in sim)" << endl;
}
