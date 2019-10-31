#include "R3BSofSciCalData.h"

// For SofSci Cal level :
// * from the fine and coarse times calculate the raw time
// * select hit by checking the raw positions at S2, Cave C and the raw ToF

// Default Constructor
R3BSofSciCalData::R3BSofSciCalData()
    : fRawTof(0)
{
    fRawTimeNs = new TArrayD(NUM_SOFSCI * NUM_SOFSCI_PMT);
    fMeanTimeNs = new TArrayD(NUM_SOFSCI);
    fRawPos = new TArrayD(NUM_SOFSCI);
}

// Destructor
R3BSofSciCalData::~R3BSofSciCalData()
{
    if (fRawTimeNs)
    {
        delete fRawTimeNs;
        fRawTimeNs = NULL;
    }
    if (fMeanTimeNs)
    {
        delete fMeanTimeNs;
        fMeanTimeNs = NULL;
    }
    if (fRawPos)
    {
        delete fRawPos;
        fRawPos = NULL;
    }
}

ClassImp(R3BSofSciCalData)
