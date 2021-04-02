#include "R3BSofTcalPar.h"

#include "FairLogger.h"
#include "FairParamList.h"

#include "TArrayF.h"
#include "TMath.h"
#include "TString.h"

#include <iostream>

#define MAX_TCALPAR 64000

using std::cout;
using std::endl;

// ---- Standard Constructor ---------------------------------------------------
R3BSofTcalPar::R3BSofTcalPar(const char* name, const char* title, const char* context)
    : FairParGenericSet(name, title, context)
    , fNumDetectors(0)
    , fNumChannels(0)
    , fNumTcalParsPerSignal(0)
{
    fNumSignals = fNumDetectors * fNumChannels;
    fAllSignalsTcalParams = new TArrayF(MAX_TCALPAR);
		fAllClockOffsets = new TArrayF(60);
}

// ----  Destructor ------------------------------------------------------------
R3BSofTcalPar::~R3BSofTcalPar()
{
    clear();
    if (fAllSignalsTcalParams)
    {
        delete fAllSignalsTcalParams;
    }
		if (fAllClockOffsets)
		{
				delete fAllClockOffsets;
		}
}

// ----  Method clear ----------------------------------------------------------
void R3BSofTcalPar::clear()
{
    status = kFALSE;
    resetInputVersions();
}

// ----  Method putParams ------------------------------------------------------
void R3BSofTcalPar::putParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofTcalPar::putParams() called";
    if (!list)
    {
        return;
    }

    Int_t array_size;

    array_size = fNumDetectors * fNumChannels * fNumTcalParsPerSignal;
    LOG(INFO) << "R3BSofTcalPar::putParams Array Size for Vftx tcal: " << array_size;
    fAllSignalsTcalParams->Set(array_size);

    array_size = fNumDetectors * fNumChannels;
    LOG(INFO) << "Array Size for clock offset corection: " << array_size;
    fAllClockOffsets->Set(array_size);

    list->add("TcalPar", *fAllSignalsTcalParams);
    list->add("nDetectorsTcalPar", fNumDetectors);
    list->add("nChannelsTcalPar", fNumChannels);
    //list->add("nSignalsTcalPar", fNumSignals);
    list->add("nTcalParsPerSignal", fNumTcalParsPerSignal);
}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BSofTcalPar::getParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofTcalPar::getParams() called";
    Int_t array_size;
    
		if (!list)
    {
        return kFALSE;
    }
    if (!list->fill("nDetectorsTcalPar", &fNumDetectors))
    {
        return kFALSE;
    }
    if (!list->fill("nChannelsTcalPar", &fNumChannels))
    {
        return kFALSE;
    }
    //if (!list->fill("nSignalsTcalPar", &fNumSignals))
    //{
    //    return kFALSE;
    //}
    if (!list->fill("nTcalParsPerSignal", &fNumTcalParsPerSignal))
    {
        return kFALSE;
    }

		array_size = fNumDetectors * fNumChannels * fNumTcalParsPerSignal;
    LOG(INFO) << "R3BSofTcalPar::getParams Array Size for VFTX tcal par: " << array_size;
    fAllSignalsTcalParams->Set(array_size);
    if (!(list->fill("TcalPar", fAllSignalsTcalParams)))
    {
        LOG(INFO) << "---R3BSofTcalPar::getParams Could not initialize fAllSignalsTcalParams";
        return kFALSE;
    }
		
		array_size = fNumDetectors * fNumChannels;
    LOG(INFO) << "R3BSofTcalPar::getParams Array Size for clock offset: " << array_size;
    fAllClockOffsets->Set(array_size);
    if (!(list->fill("ClockOffsets", fAllClockOffsets)))
    {
        LOG(INFO) << "---R3BSofTcalPar::getParams Could not initialize fAllClockOffsets";
        return kFALSE;
    }

    return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofTcalPar::printParams()
{
    LOG(INFO) << "R3BSofTcalPar: SofTcal Parameters: ";
    Int_t array_size = fNumSignals * fNumTcalParsPerSignal;

    for (Int_t d = 0; d < fNumDetectors; d++)
    {
        for (Int_t ch = 0; ch < fNumChannels; ch++)
        {
            Int_t sig = d * fNumChannels + ch;
            cout << "--- --------------------------------------------" << endl;
            cout << "--- Vftx Tcal Param for signal number: " << sig << endl;
            cout << "---       detector " << d + 1 << endl;
            cout << "---       channel " << ch + 1 << endl;
            cout << "--- --------------------------------------------" << endl;
            /*
              for (Int_t bin = 0; bin < fNumTcalParsPerSignal; bin++)
              {
              cout << "FineTime at Bin (" << bin << ") = " << fAllSignalsTcalParams->GetAt(sig * 1000 + bin)
              << endl;
              }
            */
        }
    }
}
