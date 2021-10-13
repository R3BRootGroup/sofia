// ------------------------------------------------------------------
// -----            R3BSofTwimHitPar source file                -----
// -----       Created 24/11/19  by J.L. Rodriguez-Sanchez      -----
// ------------------------------------------------------------------

#include "R3BSofTwimHitPar.h"

#include "FairDetParIo.h"
#include "FairLogger.h"
#include "FairParamList.h"

#include "TArrayF.h"
#include "TMath.h"
#include "TString.h"

#include <iostream>

// ---- Standard Constructor ---------------------------------------------------
R3BSofTwimHitPar::R3BSofTwimHitPar(const char* name, const char* title, const char* context)
    : FairParGenericSet(name, title, context)
    , fNumParamsZFit(3)
    , fNumAnodes(16)
    , fNumSec(4)
{
    fDetZHitParams = new TArrayF(fNumSec * fNumParamsZFit); // 2 Parameters for Z (Linear fits)
    fIn_use = new TArrayI(fNumAnodes * fNumSec);
    fAnode_pos = new TArrayF(fNumAnodes * fNumSec);
    // left and right + up and down --> 4
    fTofHitParams = new TArrayF(fNumSec * 3);
}

// ----  Destructor ------------------------------------------------------------
R3BSofTwimHitPar::~R3BSofTwimHitPar()
{
    clear();
    if (fIn_use)
        delete fIn_use;
    if (fAnode_pos)
        delete fAnode_pos;
    if (fDetZHitParams)
        delete fDetZHitParams;
    if (fTofHitParams)
        delete fTofHitParams;
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

    list->add("twimHitNumSecPar", fNumSec);
    list->add("twimAnodeNumberPar", fNumAnodes);
    Int_t array_anodes = fNumAnodes * fNumSec;
    LOG(INFO) << "Array Size Anodes: " << array_anodes;
    fIn_use->Set(array_anodes);
    list->add("twinInUsePar", *fIn_use);

    fAnode_pos->Set(array_anodes);
    list->add("twimAnodePosPar", *fAnode_pos);

    list->add("twimZHitParamsFitPar", fNumParamsZFit);
    Int_t array_size = fNumSec * fNumParamsZFit;
    LOG(INFO) << "Number of parameters for charge-Z: " << array_size;
    fDetZHitParams->Set(array_size);
    list->add("twimZHitPar", *fDetZHitParams);
    // fTofHitParams->Set(2 * 28 * 3);
    fTofHitParams->Set(fNumSec * 3);
    list->add("twimvstofHitPar", *fTofHitParams);
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

    if (!list->fill("twimAnodeNumberPar", &fNumAnodes))
    {
        return kFALSE;
    }

    Int_t array_anode = fNumAnodes * fNumSec;
    LOG(INFO) << "Array Size: " << array_anode;
    fIn_use->Set(array_anode);
    if (!(list->fill("twimInUsePar", fIn_use)))
    {
        LOG(ERROR) << "---Could not initialize twimInUsePar";
        return kFALSE;
    }

    fAnode_pos->Set(array_anode);
    if (!(list->fill("twimAnodePosPar", fAnode_pos)))
    {
        LOG(ERROR) << "---Could not initialize twimAnodePosPar";
        return kFALSE;
    }

    if (!list->fill("twimZHitParamsFitPar", &fNumParamsZFit))
    {
        return kFALSE;
    }

    Int_t array_size = fNumParamsZFit * fNumSec;
    LOG(INFO) << "Array Size: " << array_size;
    fDetZHitParams->Set(array_size);

    if (!(list->fill("twimZHitPar", fDetZHitParams)))
    {
        LOG(ERROR) << "---Could not initialize twimZHitPar";
        return kFALSE;
    }

    if (!(list->fill("twimvstofHitPar", fTofHitParams)))
    {
        LOG(WARNING) << "---Could not initialize twimvstofHitPar";
        // return kFALSE;
    }

    return kTRUE;
}

// ----  Method print ----------------------------------------------------------
void R3BSofTwimHitPar::print() { printParams(); }

// ----  Method printParams ----------------------------------------------------
void R3BSofTwimHitPar::printParams()
{
    LOG(INFO) << "R3BSofTwimHitPar: twim detector Parameters";
    LOG(INFO) << "R3BSofTwimHitPar: twim anodes in use: ";

    for (Int_t s = 0; s < fNumSec; s++)
    {
        LOG(INFO) << "Section = " << s + 1;
        for (Int_t j = 0; j < fNumAnodes; j++)
        {
            LOG(INFO) << "Anode " << j + 1 << " in use " << fIn_use->GetAt(s * fNumAnodes + j)
                      << ", Position-Z: " << fAnode_pos->GetAt(j);
        }
    }

    for (Int_t s = 0; s < fNumSec; s++)
    {
        LOG(INFO) << "Section = " << s + 1;
        for (Int_t j = 0; j < fNumParamsZFit; j++)
        {
            LOG(INFO) << "FitParam(" << j << ") = " << fDetZHitParams->GetAt(j + s * fNumParamsZFit);
        }
    }

    if (fTofHitParams)
        for (Int_t s = 0; s < fNumSec; s++)
        {
            LOG(INFO) << "Section = " << s + 1;
            // for (Int_t p = 0; p < 28; p++)
            for (Int_t j = 0; j < 3; j++)
            {
                // LOG(INFO) << "Tof sci nb " << p + 1 << ": FitParam(" << j
                //       << ") = " << fTofHitParams->GetAt(j + p * 3 + s * 3 * 28);
                LOG(INFO) << "FitParam(" << j << ") = " << fTofHitParams->GetAt(j + s * 3);
            }
        }
}

ClassImp(R3BSofTwimHitPar)
