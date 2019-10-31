// -------------------------------------------------------------------------
// -----            R3BSofToFWCalData source file                      -----
// -------------------------------------------------------------------------

#include "R3BSofToFWCalData.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;

// -----   Default constructor   -------------------------------------------
R3BSofToFWCalData::R3BSofToFWCalData()
    : FairMultiLinkedData()
    , fDetector(99)
    , fPmt(99)
    , fTime(-1.)
    , fEnergy(-1.)
{
}

// -----   Standard constructor   ------------------------------------------
R3BSofToFWCalData::R3BSofToFWCalData(UChar_t ident, UShort_t pmt, Double_t time, Float_t energy)
    : FairMultiLinkedData()
    , fDetector(ident)
    , fPmt(pmt)
    , fTime(time)
    , fEnergy(energy)
{
}

// -----   Copy constructor   ------------------------------------------
R3BSofToFWCalData::R3BSofToFWCalData(const R3BSofToFWCalData& right)
    : FairMultiLinkedData(right)
    , fDetector(right.fDetector)
    , fPmt(right.fPmt)
    , fTime(right.fTime)
    , fEnergy(right.fEnergy)
{
}

// -----   Destructor   ----------------------------------------------------
R3BSofToFWCalData::~R3BSofToFWCalData() {}

// -----   Public method Print   -------------------------------------------
void R3BSofToFWCalData::Print(const Option_t* opt) const
{
    cout << "-I- R3BSofToFWCalData: Sofia ToFW Cal Data level for:" << endl;
    cout << "   fDetector " << fDetector << "  fPmt = " << fPmt << endl;
    cout << "    fTime = " << fTime << " (ns)" << endl;
    cout << "    fEnergy = " << fEnergy << " (GeV in sim)" << endl;
}
