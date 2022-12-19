#include "R3BSofSciRawTofPar.h"

#include "FairLogger.h"
#include "FairParamList.h"
#include "TArrayF.h"
#include "TMath.h"
#include "TString.h"

#define MAX_RAWTOFPAR 10

// ---- Standard Constructor ---------------------------------------------------
R3BSofSciRawTofPar::R3BSofSciRawTofPar(const char* name, const char* title, const char* context)
    : FairParGenericSet(name, title, context)
    , fNumDets(1)
    , fNumChannels(3)
    , fDetIdCaveC(1)
    , fDetIdS2(0)
    , fDetIdS8(0)
    , fNumParsPerSignal(2)
{
    fNumSignals = fNumDets - 1;
    fAllSignalsRawTofParams = new TArrayF(MAX_RAWTOFPAR);
}

// ----  Destructor ------------------------------------------------------------
R3BSofSciRawTofPar::~R3BSofSciRawTofPar()
{
    clear();
    if (fAllSignalsRawTofParams)
    {
        delete fAllSignalsRawTofParams;
    }
}

// ----  Method clear ----------------------------------------------------------
void R3BSofSciRawTofPar::clear()
{
    status = kFALSE;
    resetInputVersions();
}

// ----  Method putParams ------------------------------------------------------
void R3BSofSciRawTofPar::putParams(FairParamList* list)
{
    LOG(info) << "R3BSofSciRawTofPar::putParams() called";
    if (!list)
    {
        return;
    }

    Int_t array_size = fNumSignals * fNumParsPerSignal;
    LOG(info) << "Array Size: " << array_size;

    fAllSignalsRawTofParams->Set(array_size);

    list->add("RawTofPar", *fAllSignalsRawTofParams);
    list->add("nDetsRawTofPar", fNumDets);
    list->add("nChannelsRawTofPar", fNumChannels);
    list->add("idDetS2", fDetIdS2);
    list->add("idDetS8", fDetIdS8);
    list->add("idDetCaveC", fDetIdCaveC);
    list->add("nSignalsRawTofPar", fNumSignals);
    list->add("nRawTofParsPerSignal", fNumParsPerSignal);
}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BSofSciRawTofPar::getParams(FairParamList* list)
{
    LOG(info) << "R3BSofSciRawTofPar::getParams() called";
    if (!list)
    {
        return kFALSE;
    }
    if (!list->fill("nDetsRawTofPar", &fNumDets))
    {
        return kFALSE;
    }
    if (!list->fill("nChannelsRawTofPar", &fNumChannels))
    {
        return kFALSE;
    }
    if (!list->fill("idDetCaveC", &fDetIdCaveC))
    {
        return kFALSE;
    }
    if (!list->fill("idDetS2", &fDetIdS2))
    {
        return kFALSE;
    }
    if (!list->fill("idDetS8", &fDetIdS8))
    {
        return kFALSE;
    }
    if (!list->fill("nSignalsRawTofPar", &fNumSignals))
    {
        return kFALSE;
    }
    if (!list->fill("nRawTofParsPerSignal", &fNumParsPerSignal))
    {
        return kFALSE;
    }

    Int_t array_size = fNumSignals * fNumParsPerSignal;
    LOG(info) << "Array Size: " << array_size;
    fAllSignalsRawTofParams->Set(array_size);

    if (!(list->fill("RawTofPar", fAllSignalsRawTofParams)))
    {
        LOG(info) << "---Could not initialize fAllSignalsRawTofParams";
        return kFALSE;
    }

    return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofSciRawTofPar::printParams()
{
    LOG(info) << "R3BSofSciRawTofPar: SofSciRawTof Parameters: ";
    Int_t array_size = fNumSignals * fNumParsPerSignal;

    LOG(info) << "--- --------------------------------------------";
    LOG(info) << "--- Single Tcal RawTof Parameters :  ";
    LOG(info) << "--- --------------------------------------------";
    for (Int_t param = 0; param < array_size; param++)
    {
        LOG(info) << "LIMIT " << param << " = " << fAllSignalsRawTofParams->GetAt(param);
    }
}

ClassImp(R3BSofSciRawTofPar);
