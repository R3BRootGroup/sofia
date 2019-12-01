// ------------------------------------------------------------------
// -----            R3BSofTwimHitPar source file                -----
// -----       Created 24/11/19  by J.L. Rodriguez-Sanchez      -----
// ------------------------------------------------------------------

#include "R3BSofTwimHitPar.h"

#include "FairLogger.h"
#include "FairParamList.h"

#include "TArrayF.h"
#include "TMath.h"
#include "TString.h"

#include <iostream>

// ---- Standard Constructor ---------------------------------------------------
R3BSofTwimHitPar::R3BSofTwimHitPar(const char* name, const char* title, const char* context)
    : FairParGenericSet(name, title, context)
{
    fDetHitParams = new TArrayF(4); // 2 Parameters for Z and angle (Linear fits)
    fNumSec = 1;
    fNumParamsFit = 4; // 1st order for both, Z and angle
}

// ----  Destructor ------------------------------------------------------------
R3BSofTwimHitPar::~R3BSofTwimHitPar()
{
    clear();
    delete fDetHitParams;
}

// ----  Method clear ----------------------------------------------------------
void R3BSofTwimHitPar::clear()
{
    status = kFALSE;
    resetInputVersions();
}

// ----  Method putParams ------------------------------------------------------
void R3BSofTwimHitPar::putParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofTwimHitPar::putParams() called";
    if (!list)
    {
        return;
    }

    Int_t array_size = fNumSec * fNumParamsFit;
    LOG(INFO) << "Array Size: " << array_size;

    fDetHitParams->Set(array_size);

    list->add("twimHitPar", *fDetHitParams);
    list->add("twimHitNumSecPar", fNumSec);
    list->add("twimHitParamsFitPar", fNumParamsFit);
}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BSofTwimHitPar::getParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofTwimHitPar::getParams() called";
    if (!list)
    {
        return kFALSE;
    }

    if (!list->fill("twimHitNumSecPar", &fNumSec))
    {
        return kFALSE;
    }

    if (!list->fill("twimHitParamsFitPar", &fNumParamsFit))
    {
        return kFALSE;
    }

    Int_t array_size = fNumParamsFit * fNumSec;
    LOG(INFO) << "Array Size: " << array_size;
    fDetHitParams->Set(array_size);

    if (!(list->fill("twimHitPar", fDetHitParams)))
    {
        LOG(INFO) << "---Could not initialize twimHitPar";
        return kFALSE;
    }

    return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofTwimHitPar::printParams()
{
    LOG(INFO) << "R3BSofTwimHitPar: twim detector Parameters: ";
    Int_t array_size = fNumParamsFit * fNumSec;

    for (Int_t s = 0; s < fNumSec; s++)
    {
        LOG(INFO) << "Section = " << s + 1;
        for (Int_t j = 0; j < fNumParamsFit; j++)
        {
            LOG(INFO) << "FitParam(" << j << ") = " << fDetHitParams->GetAt(j + fNumSec * fNumParamsFit);
        }
    }
}
