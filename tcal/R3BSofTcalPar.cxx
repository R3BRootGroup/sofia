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
    , fNumDetectors(30)
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
    LOG(info) << "R3BSofTcalPar::putParams() called";
    if (!list)
    {
        return;
    }

    Int_t array_size;

    array_size = fNumDetectors * fNumChannels * fNumTcalParsPerSignal;
    LOG(info) << "R3BSofTcalPar::putParams Array Size for Vftx tcal: " << array_size;
    fAllSignalsTcalParams->Set(array_size);

    array_size = fNumDetectors * fNumChannels;
    LOG(info) << "Array Size for clock offset corection: " << array_size;
    fAllClockOffsets->Set(array_size);

    list->add("TcalPar", *fAllSignalsTcalParams);
    list->add("ClockOffsets", *fAllClockOffsets);
    list->add("nDetectorsTcalPar", fNumDetectors);
    list->add("nChannelsTcalPar", fNumChannels);
    list->add("nTcalParsPerSignal", fNumTcalParsPerSignal);
}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BSofTcalPar::getParams(FairParamList* list)
{
    LOG(info) << "R3BSofTcalPar::getParams() called";
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
    LOG(info) << "R3BSofTcalPar::getParams Array Size for VFTX tcal par: " << array_size;
    fAllSignalsTcalParams->Set(array_size);
    if (!(list->fill("TcalPar", fAllSignalsTcalParams)))
    {
        LOG(error) << "---R3BSofTcalPar::getParams Could not initialize fAllSignalsTcalParams";
        return kFALSE;
    }

    array_size = fNumDetectors * fNumChannels;
    LOG(info) << "R3BSofTcalPar::getParams Array Size for clock offset: " << array_size;
    fAllClockOffsets->Set(array_size);
    if (!(list->fill("ClockOffsets", fAllClockOffsets)))
    {
        LOG(error) << "---R3BSofTcalPar::getParams Could not initialize fAllClockOffsets";
        return kFALSE;
    }

    return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofTcalPar::printParams()
{
    LOG(info) << "R3BSofTcalPar: SofTcal Parameters: ";
    Int_t array_size = (fNumDetectors * fNumChannels) * fNumTcalParsPerSignal;

    for (Int_t d = 0; d < fNumDetectors; d++)
    {
        for (Int_t ch = 0; ch < fNumChannels; ch++)
        {
            Int_t sig = d * fNumChannels + ch;
            LOG(info) << "--- --------------------------------------------";
            LOG(info) << "--- Vftx Tcal Param for signal number: " << sig;
            LOG(info) << "---       detector " << d + 1;
            LOG(info) << "---       channel " << ch + 1;
            LOG(info) << "--- --------------------------------------------";

            for (Int_t bin = 0; bin < fNumTcalParsPerSignal; bin++)
            {
                LOG(debug) << "FineTime at Bin (" << bin
                           << ") = " << fAllSignalsTcalParams->GetAt(sig * fNumTcalParsPerSignal + bin);
            }
        }
    }
}

ClassImp(R3BSofTcalPar);
