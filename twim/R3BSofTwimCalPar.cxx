// ------------------------------------------------------------------
// -----             R3BSofTwimCalPar source file               -----
// -----         Created 24/11/19  by J.L. Rodriguez-Sanchez    -----
// ------------------------------------------------------------------

#include "R3BSofTwimCalPar.h"

#include "FairLogger.h"
#include "FairParamList.h"

#include "TArrayF.h"
#include "TMath.h"
#include "TString.h"

#include <iostream>

// ---- Standard Constructor ---------------------------------------------------
R3BSofTwimCalPar::R3BSofTwimCalPar(const char* name, const char* title, const char* context)
    : FairParGenericSet(name, title, context)
{
    fAnodeCalParams = new TArrayF(192); // 16 anodes * 3 Calibration-Parameters * 4 sections
    fNumAnodes = 16;
    fNumSections = 4;
    fNumParamsFit = 3; // Gaussian fit
}

// ----  Destructor ------------------------------------------------------------
R3BSofTwimCalPar::~R3BSofTwimCalPar()
{
    clear();
    delete fAnodeCalParams;
}

// ----  Method clear ----------------------------------------------------------
void R3BSofTwimCalPar::clear()
{
    status = kFALSE;
    resetInputVersions();
}

// ----  Method putParams ------------------------------------------------------
void R3BSofTwimCalPar::putParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofTwimCalPar::putParams() called";
    if (!list)
    {
        return;
    }

    Int_t array_size = fNumSections * fNumAnodes * fNumParamsFit;
    LOG(INFO) << "Array Size: " << array_size;

    fAnodeCalParams->Set(array_size);

    list->add("twimCalPar", *fAnodeCalParams);
    list->add("twimSectionNumberPar", fNumSections);
    list->add("twimAnodeNumberPar", fNumAnodes);
    list->add("twimAnodeParamsFitPar", fNumParamsFit);
}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BSofTwimCalPar::getParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofTwimCalPar::getParams() called";
    if (!list)
    {
        return kFALSE;
    }

    if (!list->fill("twimSectionNumberPar", &fNumSections))
    {
        return kFALSE;
    }

    if (!list->fill("twimAnodeNumberPar", &fNumAnodes))
    {
        return kFALSE;
    }

    if (!list->fill("twimAnodeParamsFitPar", &fNumParamsFit))
    {
        return kFALSE;
    }

    Int_t array_size = fNumSections * fNumAnodes * fNumParamsFit;
    LOG(INFO) << "Array Size: " << array_size;
    fAnodeCalParams->Set(array_size);

    if (!(list->fill("twimCalPar", fAnodeCalParams)))
    {
        LOG(INFO) << "---Could not initialize twimCalPar";
        return kFALSE;
    }

    return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofTwimCalPar::printParams()
{
    LOG(INFO) << "R3BSofTwimCalPar: twim anode Parameters: ";
    Int_t array_size = fNumSections * fNumAnodes * fNumParamsFit;

    for (Int_t s = 0; s < fNumSections; s++)
    {
        LOG(INFO) << "Twim section: " << s;
        for (Int_t i = 0; i < fNumAnodes; i++)
        {
            LOG(INFO) << "Anode number: " << i;
            for (Int_t j = 0; j < fNumParamsFit; j++)
            {
                LOG(INFO) << "FitParam(" << j
                          << ") = " << fAnodeCalParams->GetAt(s * fNumParamsFit * fNumAnodes + i * fNumParamsFit + j);
            }
        }
    }
}
