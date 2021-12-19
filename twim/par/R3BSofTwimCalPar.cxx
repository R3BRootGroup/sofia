// ------------------------------------------------------------------
// -----             R3BSofTwimCalPar source file               -----
// -----         Created 24/11/19  by J.L. Rodriguez-Sanchez    -----
// ------------------------------------------------------------------

#include "R3BSofTwimCalPar.h"

#include "FairDetParIo.h"
#include "FairLogger.h"
#include "FairParamList.h"

#include "TArrayF.h"
#include "TArrayI.h"
#include "TMath.h"
#include "TString.h"

#include <iostream>

// ---- Standard Constructor ---------------------------------------------------
R3BSofTwimCalPar::R3BSofTwimCalPar(const char* name, const char* title, const char* context)
    : FairParGenericSet(name, title, context)
    , fNumSections(4)
    , fNumAnodes(16)
    , fNumParamsEFit(3) // Gaussian fit
    , fNumParamsPosFit(2)
{
    fAnodeCalParams = new TArrayF(fNumSections * fNumAnodes * fNumParamsEFit);
    fPosParams = new TArrayF(fNumSections * fNumAnodes * fNumParamsPosFit);
    fIn_use = new TArrayI(fNumSections * fNumAnodes);
}

// ----  Destructor ------------------------------------------------------------
R3BSofTwimCalPar::~R3BSofTwimCalPar()
{
    clear();
    if (fIn_use)
        delete fIn_use;
    if (fAnodeCalParams)
        delete fAnodeCalParams;
    if (fPosParams)
        delete fPosParams;
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

    list->add("twimSectionNumberPar", fNumSections);
    list->add("twimAnodeNumberPar", fNumAnodes);
    list->add("twimAnodeEFitPar", fNumParamsEFit);

    fIn_use->Set(fNumSections * fNumAnodes);
    list->add("twimInUsePar", *fIn_use);

    Int_t array_size = fNumSections * fNumAnodes * fNumParamsEFit;
    LOG(INFO) << "Array Size Energy: " << array_size;
    fAnodeCalParams->Set(array_size);
    list->add("twimCalEPar", *fAnodeCalParams);

    list->add("twimPosFitPar", fNumParamsPosFit);
    Int_t array_pos = fNumSections * fNumAnodes * fNumParamsPosFit;
    LOG(INFO) << "Array Size Position: " << array_pos;
    fPosParams->Set(array_pos);
    list->add("twimPosPar", *fPosParams);
}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BSofTwimCalPar::getParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofTwimCalPar::getParams() called";
    if (!list)
    {
        LOG(FATAL) << "Could not find FairParamList";
        return kFALSE;
    }

    if (!list->fill("twimSectionNumberPar", &fNumSections))
    {
        LOG(ERROR) << "Could not initialize twimSectionNumberPar";
        return kFALSE;
    }

    if (!list->fill("twimAnodeNumberPar", &fNumAnodes))
    {
        LOG(ERROR) << "Could not initialize twimAnodeNumberPar";
        return kFALSE;
    }

    if (!list->fill("twimAnodeEFitPar", &fNumParamsEFit))
    {
        LOG(ERROR) << "Could not initialize twimAnodeEFitPar";
        return kFALSE;
    }

    Int_t array_anode = fNumSections * fNumAnodes;
    LOG(INFO) << "Array Size in use: " << array_anode;
    fIn_use->Set(array_anode);
    if (!(list->fill("twimInUsePar", fIn_use)))
    {
        LOG(ERROR) << "Could not initialize twimInUsePar";
        return kFALSE;
    }

    Int_t array_size = fNumSections * fNumAnodes * fNumParamsEFit;
    LOG(INFO) << "Parameters for E calibration: " << array_size;
    fAnodeCalParams->Set(array_size);
    if (!(list->fill("twimCalEPar", fAnodeCalParams)))
    {
        LOG(ERROR) << "Could not initialize twimCalEPar";
        return kFALSE;
    }

    if (!list->fill("twimPosFitPar", &fNumParamsPosFit))
    {
        LOG(ERROR) << "Could not initialize twimPosFitPar";
        return kFALSE;
    }

    Int_t array_pos = fNumSections * fNumAnodes * fNumParamsPosFit;
    LOG(INFO) << "Parameters for Position calibration: " << array_pos;
    fPosParams->Set(array_pos);

    if (!(list->fill("twimPosPar", fPosParams)))
    {
        LOG(ERROR) << "Could not initialize twimPosPar";
        return kFALSE;
    }

    return kTRUE;
}

// ----  Method print ----------------------------------------------------------
void R3BSofTwimCalPar::print() { printParams(); }

// ----  Method printParams ----------------------------------------------------
void R3BSofTwimCalPar::printParams()
{
    LOG(INFO) << "R3BSofTwimCalPar::printParams() twim anode parameters for energy: ";
    Int_t array_size = fNumSections * fNumAnodes * fNumParamsEFit;

    for (Int_t s = 0; s < fNumSections; s++)
    {
        LOG(INFO) << "Twim section: " << s;
        for (Int_t i = 0; i < fNumAnodes; i++)
        {
            LOG(INFO) << "Anode number: " << i;
            for (Int_t j = 0; j < fNumParamsEFit; j++)
            {
                LOG(INFO) << "FitParam(" << j
                          << ") = " << fAnodeCalParams->GetAt(s * fNumParamsEFit * fNumAnodes + i * fNumParamsEFit + j);
            }
        }
    }

    LOG(INFO) << "R3BSofTwimCalPar::printParams() twim anode parameters for position: "
              << fNumSections * fNumAnodes * fNumParamsPosFit;
    for (Int_t s = 0; s < fNumSections; s++)
    {
        LOG(INFO) << "Twim section: " << s;
        for (Int_t i = 0; i < fNumAnodes; i++)
        {
            LOG(INFO) << "Anode number: " << i;
            for (Int_t j = 0; j < fNumParamsPosFit; j++)
            {
                LOG(INFO) << "FitParam(" << j
                          << ") = " << fPosParams->GetAt(s * fNumAnodes * fNumParamsPosFit + i * fNumParamsPosFit + j);
            }
        }
    }
}

ClassImp(R3BSofTwimCalPar);
