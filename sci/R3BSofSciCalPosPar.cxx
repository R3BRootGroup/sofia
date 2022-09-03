#include "R3BSofSciCalPosPar.h"

#include "FairLogger.h"
#include "FairParamList.h"

#include "TArrayF.h"
#include "TMath.h"
#include "TString.h"

#include <iostream>

#define MAX_ALLPARS 10

// ---- Standard Constructor ---------------------------------------------------
R3BSofSciCalPosPar::R3BSofSciCalPosPar(const char* name, const char* title, const char* context)
    : FairParGenericSet(name, title, context)
    , fNumDets(0)
    , fNumParamsPerDet(2)
{
    fAllParams = new TArrayF(MAX_ALLPARS);
}

// ----  Destructor ------------------------------------------------------------
R3BSofSciCalPosPar::~R3BSofSciCalPosPar()
{
    clear();
    if (fAllParams)
    {
        delete fAllParams;
    }
}

// ----  Method clear ----------------------------------------------------------
void R3BSofSciCalPosPar::clear()
{
    status = kFALSE;
    resetInputVersions();
}

// ----  Method putParams ------------------------------------------------------
void R3BSofSciCalPosPar::putParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofSciCalPosPar::putParams() called";
    if (!list)
    {
        return;
    }

    list->add("nDets", fNumDets);
    list->add("nCalPosParsPerDet", fNumParamsPerDet);
    Int_t array_size = fNumDets * fNumParamsPerDet;
    LOG(INFO) << "Array Size: " << array_size;
    fAllParams->Set(array_size);
    list->add("CalPosPar", *fAllParams);
}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BSofSciCalPosPar::getParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofSciCalPosPar::getParams() called";
    if (!list)
    {
        return kFALSE;
    }
    if (!list->fill("nDets", &fNumDets))
    {
        return kFALSE;
    }
    if (!list->fill("nCalPosParsPerDet", &fNumParamsPerDet))
    {
        return kFALSE;
    }

    Int_t array_size = fNumDets * fNumParamsPerDet;
    LOG(INFO) << "Array Size: " << array_size;
    fAllParams->Set(array_size);

    if (!(list->fill("CalPosPar", fAllParams)))
    {
        LOG(INFO) << "---Could not initialize fAllParams";
        return kFALSE;
    }

    return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofSciCalPosPar::printParams()
{
    LOG(INFO) << "R3BSofSciCalPosPar: SofSciCalPos Parameters: ";
    Int_t array_size = fNumDets * fNumParamsPerDet;

    for (Int_t det = 0; det < fNumDets; det++)
    {
        LOG(INFO) << "SofSci" << det + 1 << ": p0 = " << fAllParams->GetAt(2 * det)
                  << " , p1=" << fAllParams->GetAt(2 * det + 1);
    }
}

ClassImp(R3BSofSciCalPosPar);
