#include "R3BSofTcalPar.h"

#include "FairDetParIo.h"
#include "FairLogger.h"
#include "FairParamList.h"

#include "TArrayF.h"
#include "TMath.h"
#include "TString.h"

#include <iostream>

// ---- Standard Constructor ---------------------------------------------------
R3BSofTcalPar::R3BSofTcalPar(const char* name, const char* title, const char* context)
    : FairParGenericSet(name, title, context)
    , fNumDetectors(1)
    , fNumChannels(3)
    , fNumTcalParsPerSignal(1000)
{
    fAllSignalsTcalParams = new TArrayF(fNumDetectors * fNumChannels * fNumTcalParsPerSignal);
    fAllClockOffsets = new TArrayF(fNumDetectors * fNumChannels);
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
    list->add("nTcalParsPerSignal", fNumTcalParsPerSignal);
    list->add("ClockOffsets", fAllClockOffsets);
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
    if (!list->fill("nTcalParsPerSignal", &fNumTcalParsPerSignal))
    {
        return kFALSE;
    }

    array_size = fNumDetectors * fNumChannels * fNumTcalParsPerSignal;
    LOG(INFO) << "R3BSofTcalPar::getParams Array Size for VFTX tcal par: " << array_size;
    fAllSignalsTcalParams->Set(array_size);
    if (!(list->fill("TcalPar", fAllSignalsTcalParams)))
    {
        LOG(ERROR) << "---R3BSofTcalPar::getParams Could not initialize fAllSignalsTcalParams";
        return kFALSE;
    }

    array_size = fNumDetectors * fNumChannels;
    LOG(INFO) << "R3BSofTcalPar::getParams Array Size for clock offset: " << array_size;
    fAllClockOffsets->Set(array_size);
    if (!(list->fill("ClockOffsets", fAllClockOffsets)))
    {
        LOG(ERROR) << "---R3BSofTcalPar::getParams Could not initialize fAllClockOffsets";
        return kFALSE;
    }

    return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofTcalPar::printParams()
{
    LOG(INFO) << "R3BSofTcalPar: SofTcal Parameters: ";
    Int_t array_size = (fNumDetectors * fNumChannels) * fNumTcalParsPerSignal;

    for (Int_t d = 0; d < fNumDetectors; d++)
    {
        for (Int_t ch = 0; ch < fNumChannels; ch++)
        {
            Int_t sig = d * fNumChannels + ch;
            LOG(INFO) << "--- --------------------------------------------";
            LOG(INFO) << "--- Vftx Tcal Param for signal number: " << sig;
            LOG(INFO) << "---       detector " << d + 1;
            LOG(INFO) << "---       channel " << ch + 1;
            LOG(INFO) << "--- --------------------------------------------";
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

ClassImp(R3BSofTcalPar)
