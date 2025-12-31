#include "R3BSofSciRawPosPar.h"

#include "FairLogger.h"
#include "FairParamList.h"
#include "TArrayF.h"
#include "TMath.h"
#include "TString.h"

#define MAX_RAWPOSPAR 10

// ---- Standard Constructor ---------------------------------------------------
R3BSofSciRawPosPar::R3BSofSciRawPosPar(const char* name, const char* title, const char* context)
    : FairParGenericSet(name, title, context)
    , fNumDets(1)
    , fNumPmts(3)
    , fNumSignals(2)
    , fNumParsPerSignal(1)
{
    fAllRawPosParams = new TArrayF(fNumSignals * fNumParsPerSignal);
}

// ----  Destructor ------------------------------------------------------------
R3BSofSciRawPosPar::~R3BSofSciRawPosPar()
{
    clear();
    if (fAllRawPosParams)
    {
        delete fAllRawPosParams;
    }
}

// ----  Method clear ----------------------------------------------------------
void R3BSofSciRawPosPar::clear()
{
    status = kFALSE;
    resetInputVersions();
}

// ----  Method putParams ------------------------------------------------------
void R3BSofSciRawPosPar::putParams(FairParamList* list)
{
    LOG(info) << "R3BSofSciRawPosPar::putParams() called";
    if (!list)
    {
        return;
    }

    Int_t array_size = fNumSignals * fNumParsPerSignal;
    LOG(info) << "Array Size: " << array_size;

    fAllRawPosParams->Set(array_size);

    list->add("RawPosPar", *fAllRawPosParams);
    list->add("nDetectorsRawPosPar", fNumDets);
    list->add("nChannelsRawPosPar", fNumPmts);
    list->add("nSignalsRawPosPar", fNumSignals);
    list->add("nRawPosParsPerSignal", fNumParsPerSignal);
}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BSofSciRawPosPar::getParams(FairParamList* list)
{
    LOG(info) << "R3BSofSciRawPosPar::getParams() called";
    if (!list)
    {
        return kFALSE;
    }
    if (!list->fill("nDetectorsRawPosPar", &fNumDets))
    {
        return kFALSE;
    }
    if (!list->fill("nChannelsRawPosPar", &fNumPmts))
    {
        return kFALSE;
    }
    if (!list->fill("nSignalsRawPosPar", &fNumSignals))
    {
        return kFALSE;
    }
    if (!list->fill("nRawPosParsPerSignal", &fNumParsPerSignal))
    {
        return kFALSE;
    }

    Int_t array_size = fNumSignals * fNumParsPerSignal;
    LOG(info) << "Array Size: " << array_size;
    fAllRawPosParams->Set(array_size);

    if (!(list->fill("RawPosPar", fAllRawPosParams)))
    {
        LOG(info) << "---Could not initialize fAllRawPosParams";
        return kFALSE;
    }

    return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofSciRawPosPar::printParams()
{
    LOG(info) << "R3BSofSciRawPosPar: SofSciRawPos Parameters: ";
    Int_t array_size = fNumSignals * fNumParsPerSignal;

    LOG(info) << "--- --------------------------------------------";
    LOG(info) << "--- Single Tcal Parameters :  ";
    LOG(info) << "--- --------------------------------------------";
    for (Int_t param = 0; param < array_size; param++)
    {
        LOG(info) << "LIMIT " << param << " = " << fAllRawPosParams->GetAt(param);
    }
}

ClassImp(R3BSofSciRawPosPar);
